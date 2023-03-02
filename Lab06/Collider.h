#pragma once
#include "Actor.h"

class Collider : public Actor
{
public:
	Collider(class Game* game, float width, float height);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
};