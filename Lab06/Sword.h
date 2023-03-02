#pragma once
#include "Actor.h"

class Sword : public Actor
{
public:
	Sword(class Game* game);

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
};