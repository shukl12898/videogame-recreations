#include "Frog.h"
#include "Game.h"
#include "SpriteComponent.h"

Frog::Frog(Game* game)
: Actor(game)
{
	mGame = game;
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(game->GetTexture("Assets/Frog.png"));
}