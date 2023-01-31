#include "DeadFrog.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Vehicle.h"

DeadFrog::DeadFrog(Game* game, Vector2 position)
: Actor(game)
{
	mGame = game;
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(game->GetTexture("Assets/Dead.png"));
	SetPosition(position);
}

void DeadFrog::OnUpdate(float deltaTime)
{
	mLifetime += deltaTime;
	if (mLifetime >= 0.5f)
	{
		SetState(ActorState::Destroy);
	}
}
