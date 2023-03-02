#pragma once
#include "Actor.h"

class Soldier : public Actor
{
public:
	Soldier(class Game* game, class PathNode* start, class PathNode* end);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class AnimatedSprite* mASC;
};