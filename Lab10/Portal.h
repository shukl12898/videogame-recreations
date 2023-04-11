#pragma once
#include "Actor.h"

class Portal : public Actor
{
public:
	Portal(class Game* game);

private:
	class PortalMeshComponent* mPortalMesh = nullptr;
	class CollisionComponent* mCollisionComponent = nullptr;

};