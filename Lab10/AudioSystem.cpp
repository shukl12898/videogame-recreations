#include "AudioSystem.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <filesystem>

// Create the AudioSystem with specified number of channels
// (Defaults to 8 channels)
AudioSystem::AudioSystem(int numChannels)
{
	// TODO: Implement
}

// Destroy the AudioSystem
AudioSystem::~AudioSystem()
{
	// TODO: Implement
}

// Updates the status of all the active sounds every frame
void AudioSystem::Update(float deltaTime)
{
	// TODO: Implement
}

// Plays the sound with the specified name and loops if looping is true
// Returns the SoundHandle which is used to perform any other actions on the
// sound when active
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
SoundHandle AudioSystem::PlaySound(const std::string& soundName, bool looping)
{
	// TODO: Implement
	return 0;
}

// Stops the sound if it is currently playing
void AudioSystem::StopSound(SoundHandle sound)
{
	// TODO: Implement
}

// Pauses the sound if it is currently playing
void AudioSystem::PauseSound(SoundHandle sound)
{
	// TODO: Implement
}

// Resumes the sound if it is currently paused
void AudioSystem::ResumeSound(SoundHandle sound)
{
	// TODO: Implement
}

// Returns the current state of the sound
SoundState AudioSystem::GetSoundState(SoundHandle sound)
{
	// TODO: Implement
	return SoundState::Stopped;
}

// Stops all sounds on all channels
void AudioSystem::StopAllSounds()
{
	// TODO: Implement
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
