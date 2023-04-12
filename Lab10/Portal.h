#pragma once
#include "Actor.h"

class Portal : public Actor
{
public:
	Portal(class Game* game);
	void SetCollisionComponent(class CollisionComponent* cc) { mCollisionComponent = cc;};

private:
	class PortalMeshComponent* mPortalMesh = nullptr;
	class CollisionComponent* mCollisionComponent = nullptr;
};