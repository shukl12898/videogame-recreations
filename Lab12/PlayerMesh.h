#pragma once
#include "Actor.h"

class PlayerMesh : public Actor
{
public:
	PlayerMesh(class Game* game);
	void OnUpdate(float deltaTime) override;

private:
	class MeshComponent* mMeshComponent = nullptr;
};