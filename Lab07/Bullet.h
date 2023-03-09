#pragma once
#include "Actor.h"

class Bullet : public Actor
{
public:
	Bullet(class Game* game);
	~Bullet();
	void OnUpdate(float deltaTime) override;
	void Explosion(class Block* block);
	void SetSpeed(float speed) { mSpeed = speed; };
	float GetSpeed() const { return mSpeed; };

protected:
	class Game* mGame;
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	class MoveComponent* mMoveComponent;
	float mLifetime = 1.0f;
	float mSpeed = 900;
};