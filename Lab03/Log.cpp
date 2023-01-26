#include "Log.h"
#include "Game.h"
#include "SpriteComponent.h"

Log::Log(Game* game)
: Actor(game)
{
	mGame = game;
	mSpriteComponent = new SpriteComponent(this);
}

void Log::SetTexture(char classification)
{
	switch (classification)
	{
	case 'X':
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/LogX.png"));
	case 'Y':
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/LogY.png"));
	case 'Z':
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/LogZ.png"));
	}
}