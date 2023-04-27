#include "AudioSystem.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <filesystem>
#include "Game.h"
#include "Actor.h"
#include "Player.h"

// Create the AudioSystem with specified number of channels
// (Defaults to 8 channels)
AudioSystem::AudioSystem(Game* game, int numChannels)
{
	mGame = game;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(numChannels);
	mChannels.resize(numChannels);
}

// Destroy the AudioSystem
AudioSystem::~AudioSystem()
{
	for (auto it : mSounds)
	{
		Mix_FreeChunk(it.second);
	}

	mSounds.clear();
	Mix_CloseAudio();
}

// Updates the status of all the active sounds every frame
void AudioSystem::Update(float deltaTime)
{
	for (int i = 0; i < mChannels.size(); i++)
	{
		if (mChannels[i] != 0)
		{
			bool isPlaying = Mix_Playing(i);
			if (!isPlaying)
			{
				if (mHandleMap[mChannels[i]].mActor != nullptr)
				{
					mActorMap[mHandleMap[mChannels[i]].mActor].erase(mChannels[i]);
				}
				mHandleMap.erase(mChannels[i]);
				mChannels[i] = 0;
			}
			else
			{
				if (mHandleMap[mChannels[i]].mActor != nullptr)
				{
					int volume =
						CalculateVolume(mHandleMap[mChannels[i]].mActor, mGame->GetPlayer());
					Mix_Volume(i, volume);
				}
			}
		}
	}
}

// Plays the sound with the specified name and loops if looping is true
// Returns the SoundHandle which is used to perform any other actions on the
// sound when active
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
SoundHandle AudioSystem::PlaySound(const std::string& soundName, bool looping, Actor* actor,
								   bool stopOnActorRemove, int fadeTimeMS)
{
	Mix_Chunk* currentSound = GetSound(soundName);
	if (currentSound == nullptr)
	{
		SDL_Log("[AudioSystem] PlaySound couldn't find sound for %s", soundName.c_str());
		return 0;
	}

	int channelNumber = -1;
	bool overwritten = false;
	std::string overwrittenName;
	SoundHandle overwrittenHandle = 0;

	for (int i = 0; i < mChannels.size(); i++)
	{
		if (mChannels[i] == 0)
		{
			channelNumber = i;
			break;
		}
	}

	if (channelNumber == -1)
	{
		for (auto it : mHandleMap)
		{
			if (it.second.mSoundName == soundName)
			{
				overwrittenName = it.second.mSoundName;
				channelNumber = it.second.mChannel;
				overwrittenHandle = it.first;
				overwritten = true;
				break;
			}
		}

		if (channelNumber == -1)
		{
			for (auto it : mHandleMap)
			{
				if (!it.second.mIsLooping)
				{
					overwrittenName = it.second.mSoundName;
					channelNumber = it.second.mChannel;
					overwrittenHandle = it.first;
					overwritten = true;
				}
			}
		}
	}

	if (channelNumber == -1)
	{
		auto it = mHandleMap.begin();
		overwrittenName = it->second.mSoundName;
		channelNumber = it->second.mChannel;
		overwrittenHandle = it->first;
		overwritten = true;
	}

	if (overwritten)
	{
		mHandleMap.erase(overwrittenHandle);
		SDL_Log("[AudioSystem] PlaySound ran out of channels playing %s! Stopping %s",
				soundName.c_str(), overwrittenName.c_str());
	}

	mLastHandle++;
	HandleInfo currentHandleInfo;
	currentHandleInfo.mSoundName = soundName;
	currentHandleInfo.mIsLooping = looping;
	currentHandleInfo.mIsPaused = false;
	currentHandleInfo.mChannel = channelNumber;
	currentHandleInfo.mActor = actor;
	currentHandleInfo.mStopOnActorRemove = stopOnActorRemove;
	mHandleMap[mLastHandle] = currentHandleInfo;
	mChannels[channelNumber] = mLastHandle;

	if (actor != nullptr)
	{
		mActorMap[actor].insert(mLastHandle);
	}

	int isLooping = 0;
	if (looping)
	{
		isLooping = -1;
	}

	if (fadeTimeMS > 0)
	{
		Mix_FadeInChannel(channelNumber, currentSound, isLooping, fadeTimeMS);
	}
	else
	{
		Mix_PlayChannel(channelNumber, currentSound, isLooping);
	}

	int volume = CalculateVolume(actor, mGame->GetPlayer());
	Mix_Volume(channelNumber, volume);

	return mLastHandle;
}

void AudioSystem::RemoveActor(Actor* actor)
{
	if (mActorMap.find(actor) != mActorMap.end())
	{
		for (auto handle : mActorMap[actor])
		{
			if (mHandleMap.find(handle) != mHandleMap.end())
			{
				mHandleMap[handle].mActor = nullptr;
				if (mHandleMap[handle].mStopOnActorRemove &&
					Mix_Playing(mHandleMap[handle].mChannel))
				{
					Mix_HaltChannel(mHandleMap[handle].mChannel);
				}
			}
		}

		mActorMap.erase(actor);
	}
}

// Stops the sound if it is currently playing
void AudioSystem::StopSound(SoundHandle sound, int fadeTimeMS)
{
	auto it = mHandleMap.find(sound);
	if (it == mHandleMap.end())
	{
		SDL_Log("[AudioSystem] StopSound couldn't find handle %u", sound);
	}
	else
	{
		if (fadeTimeMS > 0)
		{
			Mix_FadeOutChannel(it->second.mChannel, fadeTimeMS);
		}
		else
		{
			Mix_HaltChannel(it->second.mChannel);
			mChannels[it->second.mChannel] = 0;
			mHandleMap.erase(it->first);
		}
	}
}

// Pauses the sound if it is currently playing
void AudioSystem::PauseSound(SoundHandle sound)
{
	auto it = mHandleMap.find(sound);
	if (it == mHandleMap.end())
	{
		SDL_Log("[AudioSystem] PauseSound couldn't find handle %u", sound);
	}
	else
	{

		if (!(it->second.mIsPaused))
		{
			Mix_Pause(it->second.mChannel);
			it->second.mIsPaused = true;
		}
	}
}

// Resumes the sound if it is currently paused
void AudioSystem::ResumeSound(SoundHandle sound)
{
	auto it = mHandleMap.find(sound);
	if (it == mHandleMap.end())
	{
		SDL_Log("[AudioSystem] ResumeSound couldn't find handle %u", sound);
	}
	else
	{

		if (it->second.mIsPaused)
		{
			Mix_Resume(it->second.mChannel);
			it->second.mIsPaused = false;
		}
	}
}

// Returns the current state of the sound
SoundState AudioSystem::GetSoundState(SoundHandle sound)
{
	auto it = mHandleMap.find(sound);
	if (it == mHandleMap.end())
	{
		return SoundState::Stopped;
	}

	if (it->second.mIsPaused)
	{
		return SoundState::Paused;
	}

	return SoundState::Playing;
}

// Stops all sounds on all channels
void AudioSystem::StopAllSounds()
{
	Mix_HaltChannel(-1);
	for (int i = 0; i < mChannels.size(); i++)
	{
		mChannels[i] = 0;
	}
	mHandleMap.clear();
}

// Cache all sounds under Assets/Sounds
void AudioSystem::CacheAllSounds()
{
#ifndef __clang_analyzer__
	std::error_code ec{};
	for (const auto& rootDirEntry : std::filesystem::directory_iterator{"Assets/Sounds", ec})
	{
		std::string extension = rootDirEntry.path().extension().string();
		if (extension == ".ogg" || extension == ".wav")
		{
			std::string fileName = rootDirEntry.path().stem().string();
			fileName += extension;
			CacheSound(fileName);
		}
	}
#endif
}

// Used to preload the sound data of a sound
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
void AudioSystem::CacheSound(const std::string& soundName)
{
	GetSound(soundName);
}

// If the sound is already loaded, returns Mix_Chunk from the map.
// Otherwise, will attempt to load the file and save it in the map.
// Returns nullptr if sound is not found.
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
Mix_Chunk* AudioSystem::GetSound(const std::string& soundName)
{
	std::string fileName = "Assets/Sounds/";
	fileName += soundName;

	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("[AudioSystem] Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

// Input for debugging purposes
void AudioSystem::ProcessInput(const Uint8* keyState)
{
	// Debugging code that outputs all active sounds on leading edge of period key
	if (keyState[SDL_SCANCODE_PERIOD] && !mLastDebugKey)
	{
		SDL_Log("[AudioSystem] Active Sounds:");
		for (size_t i = 0; i < mChannels.size(); i++)
		{
			if (mChannels[i] != 0)
			{
				auto iter = mHandleMap.find(mChannels[i]);
				if (iter != mHandleMap.end())
				{
					HandleInfo& hi = iter->second;
					SDL_Log("Channel %d: %u, %s, looping = %d, paused = %d",
							static_cast<unsigned>(i), mChannels[i], hi.mSoundName.c_str(),
							hi.mIsLooping, hi.mIsPaused);
				}
				else
				{
					SDL_Log("Channel %d: %u INVALID", static_cast<unsigned>(i), mChannels[i]);
				}
			}
		}
	}

	mLastDebugKey = keyState[SDL_SCANCODE_PERIOD];
}

int AudioSystem::CalculateVolume(Actor* actor, Actor* listener) const
{
	if (actor == nullptr || listener == nullptr)
	{
		return 128;
	}

	Vector3 actorPos = actor->GetWorldPosition();
	Vector3 listenerPos = listener->GetWorldPosition();
	float distance = Vector3::Distance(actorPos, listenerPos);

	if (distance >= 600.0f)
	{
		return 0;
	}
	if (distance <= 25.0f)
	{
		return 128;
	}

	float volume = 1 - (distance - 25.0f) / (600.0f - 25.0f);
	return static_cast<int>(volume);
}