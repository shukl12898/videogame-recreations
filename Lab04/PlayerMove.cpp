#include "PlayerMove.h"
#include "Player.h"
#include "Actor.h"
#include "Game.h"
#include "Block.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Math.h"

PlayerMove::PlayerMove(Player* owner)
: MoveComponent(owner)
{

	mGame = owner->GetGame();
	mOwner = owner;
	// Vector2 initialDirection(1, 0);
}

void PlayerMove::Update(float deltaTime)
{
	Vector2 newPos(mOwner->GetPosition().x + deltaTime * mForwardSpeed,
				   mOwner->GetPosition().y + mYSpeed * deltaTime);

	if (newPos.y > 448.0f)
	{
		newPos.y = 448.0f;
		mInAir = false;
	}

	mOwner->SetPosition(newPos);

	bool collision = false;
	for (Block* block : mGame->GetBlocks())
	{
		Vector2 offset(0, 0);
		CollSide result =
			mOwner->GetCollisionComponent()->GetMinOverlap(block->GetCollisionComponent(), offset);

		if (result == CollSide::Top && mYSpeed > 0.0f)
		{
			mYSpeed = 0;
			mInAir = false;
		}

		if (result == CollSide::Bottom && mYSpeed < 0.0f)
		{
			mYSpeed = 0;
		}

		if (result != CollSide::None)
		{
			mOwner->SetPosition(mOwner->GetPosition() + offset);
			collision = true;
		}
	}

	if (collision == false)
	{
		mInAir = true;
	}

	mYSpeed += GRAVITY * deltaTime;
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{

	if (keyState[SDL_SCANCODE_SPACE] && !mSpacePressed)
	{
		if (!mInAir)
		{
			mYSpeed = -700.0f;
		}
		mInAir = true;
	}

	if (keyState[SDL_SCANCODE_A])
	{
		mForwardSpeed = -300.0f;
	}
	else if (keyState[SDL_SCANCODE_D])
	{
		mForwardSpeed = 300.0f;
	}
	else
	{
		mForwardSpeed = 0;
	}

	mSpacePressed = keyState[SDL_SCANCODE_SPACE];
}