#pragma once
#include "Actor.h"

class Block : public Actor
{
public:
	Block(class Game* game, size_t textureIndex);
	~Block();

protected:
	class Game* mGame;
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
};