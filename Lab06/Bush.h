#pragma once
#include "Actor.h"

class Bush : public Actor
{
public:
	Bush(class Game* game);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class SpriteComponent* mSpriteComponent;
};