#pragma once
#include "Actor.h"
#include <string>

class Player : public Actor
{
public:
	Player(class Game* game);
	void OnUpdate(float deltaTime) override;
	bool HasGun() const { return mHasGun; };
	class PlayerUI* GetPlayerUI() { return mPlayerUI; };
	void GiveGun();
	class CameraComponent* GetCamera() { return mCameraComponent; };
	int GetDeadSound() const { return mDeadSound; }
	class HUD* GetHUD() { return mHUD; }
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
	class HUD* mHUD = nullptr;
	bool mHasGun = false;
	Vector3 mVector3;
	int mDeadSound = 0;
	std::vector<std::string> mDeathSounds;
	std::vector<std::string> mDeathSubtitles;
};