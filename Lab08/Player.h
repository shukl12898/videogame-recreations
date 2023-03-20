#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	void OnUpdate(float deltaTime) override;

protected:
	class Game* mGame;
	class MeshComponent* mMeshComp;
	class PlayerMove* mPlayerMove;
};