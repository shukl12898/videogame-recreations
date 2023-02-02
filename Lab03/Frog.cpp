#include "Frog.h"
#include "DeadFrog.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Vehicle.h"
#include "Log.h"
#include "WrappingMove.h"

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

	std::vector<Log*> copyOfLogs = mGame->GetLogs();
	Vector2 offset(0.0f, 0.0f);
	CollSide minOverlap = CollSide::None;

	Vector2 frogCurrPos = GetPosition();

	for (auto it : copyOfLogs)
	{
		minOverlap = mCollisionComponent->GetMinOverlap(it->GetCollisionComponent(), offset);
		if (minOverlap != CollSide::None)
		{
			mRiding = true;
			WrappingMove* wrappingMove = it->GetWrappingMove();
			Vector2 newPos(frogCurrPos.x, it->GetPosition().y);
			SetPosition(newPos);

			SetPosition(GetPosition() + (wrappingMove->GetMoveDirection() *
										 wrappingMove->GetForwardSpeed() * deltaTime));

			Vector2 additional(32, 0);

			if (minOverlap == CollSide::Left)
			{
				SetPosition(it->GetPosition() + offset - additional);
			}
			else if (minOverlap == CollSide::Right)
			{
				SetPosition(it->GetPosition() + offset + additional);
			}
		}
	}

	if (!mRiding)
	{

		if (frogCurrPos.y >= 180 && frogCurrPos.y <= 510)
		{
			new DeadFrog(mGame, GetPosition());
			SetPosition(mInitialPosition);
		}
	}

	bool endpoint =
		mCollisionComponent->Intersect(mGame->GetGoal()->GetComponent<CollisionComponent>());

	if (endpoint)
	{
		SetState(ActorState::Paused);
	}
	else
	{
		if (GetPosition().y < 180)
		{
			new DeadFrog(mGame, GetPosition());
			SetPosition(mInitialPosition);
		}
	}

	mRiding = false;
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
			if (possiblePosition.y <= (mGame->HEIGHT) - (64 * 4))
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