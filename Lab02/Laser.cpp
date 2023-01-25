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
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));
	mSpriteComponent = sc;
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(400.0f);
	mMoveComponent = mc;
	mGame = game;
	mState = ActorState::Paused;
}

void Laser::OnUpdate(float deltaTime)
{
	mLifetime += deltaTime;
	if (mLifetime >= 1.0f)
	{
		SetState(ActorState::Destroy);
	}

	for (auto i : mGame->mAsteroids)
	{
		if (Vector2::Distance(GetPosition(), i->GetPosition()) <= 70)
		{
			SetState(ActorState::Destroy);
			i->SetState(ActorState::Destroy);
			break;
		}
	}
}