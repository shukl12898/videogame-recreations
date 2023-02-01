#include "Vehicle.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "WrappingMove.h"
#include "CollisionComponent.h"
#include "Math.h"

Vehicle::Vehicle(Game* game)
: Actor(game)
{
	mGame = game;
	mSpriteComponent = new SpriteComponent(this);
	mWrappingMove = new WrappingMove(this);
	mWrappingMove->SetForwardSpeed(100);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(64, 64);
	mGame->AddVehicle(this);
}

Vehicle::~Vehicle()
{
	mGame->RemoveVehicle(this);
	for (Component* i : mComponents)
	{
		delete i;
	}
	mComponents.clear();
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
		mCollisionComponent->SetSize(128, 48);
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/Truck.png"));
	}
}

void Vehicle::OnUpdate(float deltaTime)
{
	Vector2 distFrog = GetPosition() - (mGame->GetFrogPosition());
	distFrog.Normalize();
	Vector2 direction = mWrappingMove->GetMoveDirection();
	direction.Normalize();

	float angle = acos(distFrog.Dot(direction, distFrog));

	if (angle <= Math::Pi / 6)
	{
		mWrappingMove->SetForwardSpeed(mWrappingMove->GetForwardSpeed() / 2);
	}
}