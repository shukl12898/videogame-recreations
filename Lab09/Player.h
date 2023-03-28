#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	void OnUpdate(float deltaTime) override;
	bool HasGun();
	class PlayerUI* GetPlayerUI() { return mPlayerUI; };
	void GiveGun();
	class CameraComponent* GetCamera() { return mCameraComponent; };

protected:
	class Game* mGame;
	class MeshComponent* mMeshComp;
	class PlayerMove* mPlayerMove;
	class CameraComponent* mCameraComponent;
	class CollisionComponent* mCollisionComponent;
	class PlayerUI* mPlayerUI;
	bool mHasGun = false;
};