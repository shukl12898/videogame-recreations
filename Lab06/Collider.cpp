#include "Collider.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"

Collider::Collider(Game* game, int width, int height)
: Actor(game)
{
	mGame = game;
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(width, height);
}
