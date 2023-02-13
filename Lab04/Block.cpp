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

	std::string prefix = "Assets/Block";
	std::string suffix = ".png";
	std::string fileName = prefix + classification + suffix;

	mSpriteComponent->SetTexture(mGame->GetTexture(fileName));

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