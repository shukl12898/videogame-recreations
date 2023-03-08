#include "Actor.h"
#include "Sword.h"
#include "Game.h"
#include "CollisionComponent.h"

Sword::Sword(Game* game)
: Actor(game)
{
	mGame = game;
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(SWORD_SIZE, SWORD_SIZE);
}