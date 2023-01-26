#include "Vehicle.h"
#include "Game.h"
#include "SpriteComponent.h"

Vehicle::Vehicle(Game* game)
: Actor(game)
{
	mGame = game;
	mSpriteComponent = new SpriteComponent(this);
}

void Vehicle::SetTexture(char classification)
{
	switch (classification)
	{
	case 'A':
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/CarA.png"));
	case 'B':
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/CarB.png"));
	case 'C':
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/CarC.png"));
	case 'D':
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/CarD.png"));
	case 'T':
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/Truck.png"));
	}
}