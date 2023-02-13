#include "PlayerMove.h"
#include "Player.h"
#include "Actor.h"
#include "Game.h"
#include "Block.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Math.h"
#include "Goomba.h"
#include <string>

PlayerMove::PlayerMove(Player* owner)
: MoveComponent(owner)
{

	mGame = owner->GetGame();
	mOwner = owner;
}

void PlayerMove::Update(float deltaTime)
{

	bool dead = false;
	bool won = false;

	Vector2 newPos(mOwner->GetPosition().x + deltaTime * mForwardSpeed,
				   mOwner->GetPosition().y + mYSpeed * deltaTime);

	if (newPos.x < mGame->GetCameraPos().x)
	{
		newPos.x = mGame->GetCameraPos().x;
	}

	if (newPos.x > WIN_LOCATION)
	{
		won = true;
	}

	if (newPos.y >= IN_DITCH)
	{
		dead = true;
		newPos.y = FALL_THROUGH;
		mOwner->SetState(ActorState::Paused);
		mInAir = false;
	}

	mOwner->SetPosition(newPos);

	bool collision = false;
	CollisionComponent* collComponent = mOwner->GetCollisionComponent();

	for (Block* block : mGame->GetBlocks())
	{
		Vector2 offset(0, 0);
		CollSide result = collComponent->GetMinOverlap(block->GetCollisionComponent(), offset);
		Mix_Chunk* bump = mGame->GetSound("Assets/Sounds/Bump.wav");

		if (result == CollSide::Top && mYSpeed > 0.0f)
		{
			mYSpeed = 0;
			mInAir = false;
		}

		if (result == CollSide::Bottom && mYSpeed < 0.0f)
		{
			Mix_PlayChannel(-1, bump, 0);
			mYSpeed = 0;
		}

		if (result != CollSide::None)
		{
			mOwner->SetPosition(mOwner->GetPosition() + offset);
			collision = true;
		}
	}

	if (!collision)
	{
		mInAir = true;
	}

	for (Goomba* goomba : mGame->GetGoombas())
	{

		Vector2 offset(0, 0);
		CollSide result = collComponent->GetMinOverlap(goomba->GetCollisionComponent(), offset);
		Mix_Chunk* stomp = mGame->GetSound("Assets/Sounds/Stomp.wav");

		if (!goomba->GetStomped())
		{
			if (result == CollSide::Top)
			{
				Mix_PlayChannel(-1, stomp, 0);
				goomba->SetStomped();
				mYSpeed = -HALF_JUMP;
				mInAir = true;
			}
			else if ((result == CollSide::Left || result == CollSide::Right) && mInAir)
			{
				Mix_PlayChannel(-1, stomp, 0);
				goomba->SetStomped();
				mYSpeed = -HALF_JUMP;
				mInAir = true;
			}
			else if (result != CollSide::None)
			{
				dead = true;
				mOwner->SetState(ActorState::Paused);
			}

			if (result != CollSide::None)
			{
				mOwner->SetPosition(mOwner->GetPosition() + offset);
			}
		}
	}

	mYSpeed += GRAVITY * deltaTime;

	float newCameraPos = mOwner->GetPosition().x - (mGame->WIDTH / 2);

	if (newCameraPos >= mGame->GetCameraPos().x && newCameraPos >= 0)
	{
		mGame->GetCameraPos().x = newCameraPos;
	}

	std::string animation = DetermineAnimation(mInAir);

	if (dead)
	{
		animation = "dead";
		Mix_Chunk* deadSound = mGame->GetSound("Assets/Sounds/Dead.wav");
		Mix_PlayChannel(-1, deadSound, 0);
		Mix_HaltChannel(mGame->GetBackgroundMusicChannel());
	}

	if (won)
	{
		Mix_HaltChannel(mGame->GetBackgroundMusicChannel());
		Mix_Chunk* wonSound = mGame->GetSound("Assets/Sounds/StageClear.wav");
		Mix_PlayChannel(-1, wonSound, 0);
		mOwner->SetState(ActorState::Paused);
	}

	mOwner->GetAnimatedSprite()->SetAnimation(animation);
}

std::string PlayerMove::DetermineAnimation(bool jump)
{
	std::string animation;

	//mario on ground
	if (!jump)
	{
		//moving right
		if (mForwardSpeed > 0)
		{
			animation = "runRight";
		}
		//moving left
		else if (mForwardSpeed < 0)
		{
			animation = "runLeft";
		}
		//not moving right or left
		else
		{
			animation = "idle";
		}
	}
	//mario in air
	else
	{
		//moving right
		if (mForwardSpeed > 0)
		{
			animation = "jumpRight";
		}
		//moving left
		else if (mForwardSpeed < 0)
		{
			animation = "jumpLeft";
		}
		else
		{
			std::string current = mOwner->GetAnimatedSprite()->GetAnimName();
			if ((current == "runRight") || (current == "jumpRight") || (current == "idle"))
			{
				animation = "jumpRight";
			}
			else
			{
				animation = "jumpLeft";
			}
		}
	}

	return animation;
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{

	if (keyState[SDL_SCANCODE_SPACE] && !mSpacePressed)
	{
		if (!mInAir)
		{
			Mix_Chunk* jump = mGame->GetSound("Assets/Sounds/Jump.wav");
			Mix_PlayChannel(-1, jump, 0);
			mYSpeed = -NORMAL_JUMP;
		}
		mInAir = true;
	}

	if (keyState[SDL_SCANCODE_A])
	{
		mForwardSpeed = -MARIO_SPEED;
	}
	else if (keyState[SDL_SCANCODE_D])
	{
		mForwardSpeed = MARIO_SPEED;
	}
	else
	{
		mForwardSpeed = 0;
	}

	mSpacePressed = keyState[SDL_SCANCODE_SPACE];
}