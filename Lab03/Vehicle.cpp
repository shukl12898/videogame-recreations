#include "Vehicle.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "WrappingMove.h"

Vehicle::Vehicle(Game* game)
: Actor(game)
{
	mGame = game;
	mSpriteComponent = new SpriteComponent(this);
	mWrappingMove = new WrappingMove(this);
	mWrappingMove->SetForwardSpeed(100);
}

void Vehicle::SetTexture(char classification)
{
	if (classification == 'A')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/CarA.png"));
	}
	else if (classification == 'B')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/CarB.png"));
	}
	else if (classification == 'C')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/CarC.png"));
	}
	else if (classification == 'D')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/CarD.png"));
	}
	else if (classification == 'T')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/Truck.png"));
	}
}