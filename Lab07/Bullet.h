#pragma once
#include "Actor.h"

class Bullet : public Actor
{
public:
	Bullet(class Game* game);
	~Bullet();
	void OnUpdate(float deltaTime) override;
	void Explosion();

protected:
	class Game* mGame;
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	class MoveComponent* mMoveComponent;
	float mLifetime = 1.0f;
};