#include "Ship.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"

Ship::Ship(Game* game)
: Actor(game)
{
	mGame = game;
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));
	mSpriteComponent = sc;
	MoveComponent* mc = new MoveComponent(this);
	mMoveComponent = mc;
}

void Ship::OnProcessInput(const Uint8* keyState)
{
	if (keyState[SDL_SCANCODE_S])
	{
		mMoveComponent->SetForwardSpeed(-mForwardSpeed);
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	}
	else if (keyState[SDL_SCANCODE_W])
	{
		mMoveComponent->SetForwardSpeed(mForwardSpeed);
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	}
	else
	{
		mMoveComponent->SetForwardSpeed(0);
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/Ship.png"));
	}

	if (keyState[SDL_SCANCODE_A])
	{
		mMoveComponent->SetAngularSpeed(mAngularSpeed);
	}
	else if (keyState[SDL_SCANCODE_D])
	{
		mMoveComponent->SetAngularSpeed(-mAngularSpeed);
	}
	else
	{
		mMoveComponent->SetAngularSpeed(0);
	}
}