#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	void OnUpdate(float deltaTime) override;
	class PlayerUI* GetPlayerUI() { return mPlayerUI; };

protected:
	class Game* mGame;
	class MeshComponent* mMeshComp;
	class PlayerMove* mPlayerMove;
	class CameraComponent* mCameraComponent;
	class PlayerUI* mPlayerUI;
};