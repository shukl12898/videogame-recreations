#pragma once
#include "Actor.h"

class Soldier : public Actor
{
public:
	Soldier(class Game* game);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class AnimatedSprite* mASC;
};