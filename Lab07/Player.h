#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };
	static const int SHIP_SPEED = 300;

protected:
	class Game* mGame;
	class MeshComponent* mMeshComp;
	class CollisionComponent* mCollisionComponent;
	class PlayerMove* mPlayerMove;
};