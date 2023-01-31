#include "Frog.h"
#include "DeadFrog.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Vehicle.h"

Frog::Frog(Game* game)
: Actor(game)
{
	mGame = game;
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(game->GetTexture("Assets/Frog.png"));
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(50, 50);
}

void Frog::OnUpdate(float deltaTime)
{
	std::vector<Vehicle*> copyOfVehicles = mGame->GetVehicles();

	for (auto it : copyOfVehicles)
	{
		if (mCollisionComponent->Intersect(it->GetCollisionComponent()))
		{
			new DeadFrog(mGame, GetPosition());
			SetPosition(mInitialPosition);
		}
	}
}

void Frog::OnProcessInput(const Uint8* keyState)
{
	Vector2 min(0, 0);
	Vector2 max(mGame->WIDTH, mGame->HEIGHT);

	if (keyState[SDL_SCANCODE_W])
	{
		if (mLastKeyFrame[SDL_SCANCODE_W] == false)
		{
			Vector2 newPosition(0, -64);
			Vector2 possiblePosition = GetPosition() + newPosition;
			if (possiblePosition.y >= 0)
			{
				mPosition = possiblePosition;
			}
		}
		mLastKeyFrame[SDL_SCANCODE_W] = true;
	}
	else
	{
		mLastKeyFrame[SDL_SCANCODE_W] = false;
	}
	if (keyState[SDL_SCANCODE_S])
	{
		if (mLastKeyFrame[SDL_SCANCODE_S] == false)
		{
			Vector2 newPosition(0, 64);
			Vector2 possiblePosition = GetPosition() + newPosition;
			if (possiblePosition.y <= (mGame->HEIGHT) - 64)
			{
				mPosition = possiblePosition;
			}
		}
		mLastKeyFrame[SDL_SCANCODE_S] = true;
	}
	else
	{
		mLastKeyFrame[SDL_SCANCODE_S] = false;
	}
	if (keyState[SDL_SCANCODE_A])
	{
		if (mLastKeyFrame[SDL_SCANCODE_A] == false)
		{
			Vector2 newPosition(-64, 0);
			Vector2 possiblePosition = GetPosition() + newPosition;
			if (possiblePosition.x >= 0)
			{
				mPosition = possiblePosition;
			}
		}

		mLastKeyFrame[SDL_SCANCODE_A] = true;
	}
	else
	{
		mLastKeyFrame[SDL_SCANCODE_A] = false;
	}
	if (keyState[SDL_SCANCODE_D])
	{
		if (mLastKeyFrame[SDL_SCANCODE_D] == false)
		{
			Vector2 newPosition(64, 0);
			Vector2 possiblePosition = GetPosition() + newPosition;
			if (possiblePosition.x <= mGame->WIDTH)
			{
				mPosition = possiblePosition;
			}
		}
		mLastKeyFrame[SDL_SCANCODE_D] = true;
	}
	else
	{
		mLastKeyFrame[SDL_SCANCODE_D] = false;
	}
}