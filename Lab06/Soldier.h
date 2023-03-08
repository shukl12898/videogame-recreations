#pragma once
#include "Actor.h"

class Soldier : public Actor
{
public:
	Soldier(class Game* game, class PathNode* start, class PathNode* end);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };
	bool mStunned = false;
	static const int SOLDIER_SIZE = 32;
	static const int SOLDIER_HIT_POINTS = 2;

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class AnimatedSprite* mASC;
	class EnemyComponent* mEnemyComponent;
	class SoldierAI* mSoldierAI;
};