#include "Laser.h"
#include "Ship.h"
#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"

Laser::Laser(Game* game)
: Actor(game)
{
	mGame = game;
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(game->GetTexture("Assets/Laser.png"));
	mMoveComponent = new MoveComponent(this);
	mMoveComponent->SetForwardSpeed(400.0f);
	mGame = game;

	//Ensures initial state of laser is paused
	mState = ActorState::Paused;
}

void Laser::OnUpdate(float deltaTime)
{

	//Updates lifetime based on delta time
	mLifetime += deltaTime;

	//Destroys if lifetime has exceeded the laser's max lifetime
	if (mLifetime >= MAX_LASER_LIFETIME)
	{
		SetState(ActorState::Destroy);
	}

	//Determines whether any asteroid is hit by laser, if so, destroys them
	for (Asteroid* i : mGame->GetAsteroids())
	{
		if (Vector2::Distance(GetPosition(), i->GetPosition()) <= LASER_EFFECTIVE_DISTANCE)
		{
			SetState(ActorState::Destroy);
			i->SetState(ActorState::Destroy);
			break;
		}
	}
}