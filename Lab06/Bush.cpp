#include "Bush.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"

Bush::Bush(Game* game)
: Actor(game)
{
	mGame = game;
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(32, 32);
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(mGame->GetTexture("Assets/Bush.png"));
}
