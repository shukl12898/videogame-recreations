#pragma once
#include "Actor.h"

class Sword : public Actor
{
public:
	Sword(class Game* game);
	static const int SWORD_SIZE = 28;

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
};