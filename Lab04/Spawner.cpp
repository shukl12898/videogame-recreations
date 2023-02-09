#include "Spawner.h"
#include "Game.h"
#include "Goomba.h"
#include "Player.h"

Spawner::Spawner(Game* game)
: Actor(game)
{
	mGame = game;
}

void Spawner::OnUpdate(float deltaTime)
{
	if (mPosition.x - mGame->GetPlayer()->GetPosition().x < 600)
	{
		Goomba* goomba = new Goomba(mGame);
		goomba->SetPosition(mPosition);
		SetState(ActorState::Destroy);
	}
}