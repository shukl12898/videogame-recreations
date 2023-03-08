#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };
	class AnimatedSprite* GetAnimatedSprite() { return mASC; };
	static const int PLAYER_SIZE = 20;

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class AnimatedSprite* mASC;
	class PlayerMove* mPlayerMove;
};