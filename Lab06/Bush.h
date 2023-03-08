#pragma once
#include "Actor.h"

class Bush : public Actor
{
public:
	Bush(class Game* game);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };
	static const int BUSH_SIZE = 32;
	static const int BUSH_HIT_POINTS = 1;

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class SpriteComponent* mSpriteComponent;
	class EnemyComponent* mEnemyComponent;
};