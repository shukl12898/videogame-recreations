#include "Block.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"

Block::Block(Game* game, char classification)
: Actor(game)
{
	mGame = game;
	mSpriteComponent = new SpriteComponent(this);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(32, 32);

	if (classification == 'A')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/BlockA.png"));
	}
	else if (classification == 'B')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/BlockB.png"));
	}
	else if (classification == 'C')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/BlockC.png"));
	}
	else if (classification == 'D')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/BlockD.png"));
	}
	else if (classification == 'E')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/BlockE.png"));
	}
	else if (classification == 'F')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/BlockF.png"));
	}
	else if (classification == 'G')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/BlockG.png"));
	}
	else if (classification == 'H')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/BlockH.png"));
	}
	else if (classification == 'I')
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/BlockI.png"));
	}

	mGame->AddBlock(this);
}

Block::~Block()
{
	mGame->RemoveBlock(this);
	for (Component* i : mComponents)
	{
		delete i;
	}
	mComponents.clear();
}