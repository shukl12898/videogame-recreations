#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };
	class AnimatedSprite* GetAnimatedSprite() { return mASC; };

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class AnimatedSprite* mASC;
	class PlayerMove* mPlayerMove;
};