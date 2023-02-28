#include "PlayerMove.h"
#include "Player.h"
#include "Actor.h"
#include "Game.h"
#include "Block.h"
#include "MoveComponent.h"
#include "Collider.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Math.h"
#include <string>

PlayerMove::PlayerMove(Player* owner)
: MoveComponent(owner)
{

	mGame = owner->GetGame();
	mOwner = owner;
}

void PlayerMove::Update(float deltaTime)
{

	mOwner->SetPosition(mOwner->GetPosition() + mDirection * deltaTime * MOVE_SPEED);
	mOwner->GetAnimatedSprite()->SetAnimation(DetermineAnimation());

	CollisionComponent* cc = mOwner->GetCollisionComponent();

	for (Collider* collider : mGame->GetColliders())
	{
		Vector2 offset(0, 0);
		CollSide collSide = cc->GetMinOverlap(collider->GetCollisionComponent(), offset);

		if (collSide != CollSide::None)
		{
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}

	Vector2 offset(-256.0f, -224.0f);
	mGame->SetCameraPos(mOwner->GetPosition() + offset);
}

std::string PlayerMove::DetermineAnimation()
{
	std::string animName = "";

	if (mMoving)
	{
		if (mDirectionState == DirectionState::Up)
		{
			animName = "WalkUp";
		}
		else if (mDirectionState == DirectionState::Down)
		{
			animName = "WalkDown";
		}
		else if (mDirectionState == DirectionState::Left)
		{
			animName = "WalkLeft";
		}
		else if (mDirectionState == DirectionState::Right)
		{
			animName = "WalkRight";
		}
	}
	else
	{
		if (mDirectionState == DirectionState::Up)
		{
			animName = "StandUp";
		}
		else if (mDirectionState == DirectionState::Down)
		{
			animName = "StandDown";
		}
		else if (mDirectionState == DirectionState::Left)
		{
			animName = "StandLeft";
		}
		else if (mDirectionState == DirectionState::Right)
		{
			animName = "StandRight";
		}
	}

	return animName;
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	//facing and moving up
	if (keyState[SDL_SCANCODE_W])
	{
		mDirection = Vector2(0, -1);
		mDirectionState = DirectionState::Up;
		mMoving = true;
	}
	//facing and moving down
	else if (keyState[SDL_SCANCODE_S])
	{
		mDirection = Vector2(0, 1);
		mDirectionState = DirectionState::Down;
		mMoving = true;
	}
	//facing and moving left
	else if (keyState[SDL_SCANCODE_A])
	{
		mDirection = Vector2(-1, 0);
		mDirectionState = DirectionState::Left;
		mMoving = true;
	}
	//facing and moving right
	else if (keyState[SDL_SCANCODE_D])
	{
		mDirection = Vector2(1, 0);
		mDirectionState = DirectionState::Right;
		mMoving = true;
	}

	else
	{
		mDirection = Vector2(0, 0);
		mMoving = false;
	}
}