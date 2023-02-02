#include "Log.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "WrappingMove.h"
#include "CollisionComponent.h"

Log::Log(Game* game)
: Actor(game)
{
	mGame = game;
	mSpriteComponent = new SpriteComponent(this);
	mWrappingMove = new WrappingMove(this);
	mWrappingMove->SetForwardSpeed(75);
	mCollisionComponent = new CollisionComponent(this);
	mGame->AddLog(this);
}