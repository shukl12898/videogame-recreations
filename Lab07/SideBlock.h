#pragma once
#include "Actor.h"

class SideBlock : public Actor
{
public:
	SideBlock(class Game* game, size_t textureIndex);
	void OnUpdate(float deltaTime) override;

protected:
	class Game* mGame;
	class MeshComponent* mMeshComponent;
};