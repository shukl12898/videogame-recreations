#pragma once
#include "Actor.h"

class Collider : public Actor
{
public:
	Collider(class Game* game, int width, int height);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
};