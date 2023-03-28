#pragma once
#include "Actor.h"

class PortalGun : public Actor
{
public:
	PortalGun(class Game* game);
	void OnUpdate(float deltaTime) override;

	const float ROTATION_RATE = Math::Pi;

private:
	class MeshComponent* mMeshComponent = nullptr;
	class CollisionComponent* mCollisionComponent = nullptr;
};