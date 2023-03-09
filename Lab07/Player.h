#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };
	void OnUpdate(float deltaTime) override;
	static const int SHIP_SPEED = 300;

protected:
	class Game* mGame;
	class MeshComponent* mMeshComp;
	class CollisionComponent* mCollisionComponent;
	class PlayerMove* mPlayerMove;
	float mCooldown = 0.0f;
	class HUD* mHUD;
};