#pragma once
#include "Actor.h"

class Block : public Actor
{
public:
	Block(class Game* game, char classification);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };
	~Block();

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class SpriteComponent* mSpriteComponent;
};