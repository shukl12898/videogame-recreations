#include "Actor.h"
#include "CollisionComponent.h"
#include "VOTrigger.h"
#include "Game.h"
#include "Player.h"
#include "AudioSystem.h"
#include "Door.h"
#include "HUD.h"
#include "HealthComponent.h"

VOTrigger::VOTrigger(Game* game)
: Actor(game)
{
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(1, 1, 1);
	mAudio = mGame->GetAudio();
}

void VOTrigger::OnUpdate(float deltaTime)
{

	if (mGame->GetPlayer()->GetComponent<HealthComponent>()->IsDead())
	{
		if (mAudio->GetSoundState(mSoundHandle) != SoundState::Stopped)
		{
			mAudio->StopSound(mSoundHandle);
		}
	}
	else
	{

		bool moreSounds = true;

		if (!mActivated)
		{
			CollisionComponent* playerCC = mGame->GetPlayer()->GetComponent<CollisionComponent>();
			if (mCollisionComponent->Intersect(playerCC))
			{
				mActivated = true;
				PlayNextSound();
			}
		}
		else
		{
			if (mAudio->GetSoundState(mSoundHandle) == SoundState::Stopped)
			{
				moreSounds = PlayNextSound();
			}

			if (!moreSounds)
			{
				if (!mDoorName.empty())
				{
					Door* door = mGame->GetDoorNames()[mDoorName];
					door->OpenDoor();
				}
				if (!mNextLevel.empty())
				{
					mGame->SetNextLevel(mNextLevel);
				}
				SetState(ActorState::Destroy);
			}
		}
	}
}

bool VOTrigger::PlayNextSound()
{
	if (mCurrentSound < mSounds.size())
	{
		mSoundHandle = mAudio->PlaySound(mSounds[mCurrentSound]);
		mGame->GetPlayer()->GetHUD()->ShowSubtitle(mSubtitles[mCurrentSound]);
		mCurrentSound++;
		return true;
	}

	mGame->GetPlayer()->GetHUD()->ShowSubtitle("");

	return false;
}

void VOTrigger::OnProcessInput(const Uint8* keyState, Uint32 mouseButtons,
							   const Vector2& relativeMouse)
{
	if (!mLastFrameF && keyState[SDL_SCANCODE_F])
	{
		if (mAudio->GetSoundState(mSoundHandle) != SoundState::Stopped)
		{
			mAudio->StopSound(mSoundHandle);
			mCurrentSound++;
			PlayNextSound();
		}
	}

	mLastFrameF = keyState[SDL_SCANCODE_F];
}