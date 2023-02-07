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

void Log::SetTexture(char classification)
{
	if (classification == 'X')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/LogX.png"));
		mCollisionComponent->SetSize(192.0f, 48.0f);
	}
	else if (classification == 'Y')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/LogY.png"));
		mCollisionComponent->SetSize(256.0f, 48.0f);
	}
	else if (classification == 'Z')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/LogZ.png"));
		mCollisionComponent->SetSize(384.0f, 48.0f);
	}
}