#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	void OnUpdate(float deltaTime) override;
	bool HasGun() const { return mHasGun; };
	class PlayerUI* GetPlayerUI() { return mPlayerUI; };
	void GiveGun();
	class CameraComponent* GetCamera() { return mCameraComponent; };
	Vector3 GetVector3() { return mVector3; };
	void SetVector3(Vector3 vec) { mVector3 = vec; };

protected:
	class Game* mGame;
	class MeshComponent* mMeshComp = nullptr;
	class PlayerMove* mPlayerMove = nullptr;
	class CameraComponent* mCameraComponent = nullptr;
	class CollisionComponent* mCollisionComponent = nullptr;
	class PlayerUI* mPlayerUI = nullptr;
	class HealthComponent* mHealthComponent = nullptr;
	bool mHasGun = false;
	Vector3 mVector3;
};