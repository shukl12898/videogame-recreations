#pragma once
#include "Actor.h"

class Block : public Actor
{
public:
	Block(class Game* game, size_t textureIndex);
	~Block();
	void OnUpdate(float deltaTime) override;
	bool GetIsExploding() const { return mIsExploding; };

protected:
	class Game* mGame;
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	bool mIsExploding = false;
};