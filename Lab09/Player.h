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
	Vector3 GetVector3() { return mVector3; };
	void SetVector3(Vector3 vec) { mVector3 = vec; };

protected:
	class Game* mGame;
	class MeshComponent* mMeshComp;
	class PlayerMove* mPlayerMove;
	class CameraComponent* mCameraComponent;
	class CollisionComponent* mCollisionComponent;
	class PlayerUI* mPlayerUI;
	bool mHasGun = false;
	Vector3 mVector3;
};