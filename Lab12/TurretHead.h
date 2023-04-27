#include "Actor.h"
#include <unordered_map>
#include <string>

enum class TurretState
{
	Idle,
	Search,
	Priming,
	Firing,
	Falling,
	Dead
};

class TurretHead : public Actor
{
public:
	TurretHead(Game* game, Actor* parent);
	bool TargetAcquistion();
	bool PortalTeleport(float deltaTime);
	void UpdateIdle(float deltaTime);
	void UpdateSearch(float deltaTime);
	void UpdatePriming(float deltaTime);
	void UpdateFiring(float deltaTime);
	void UpdateFalling(float deltaTime);
	void UpdateDead(float deltaTime);
	void ChangeState(TurretState newState);
	void Die();
	void OnUpdate(float deltaTime) override;
	void TakeDamage();

	static const int SIDE_DIST = 75;
	static const int UP_DIST = 25;
	static const int FWD_DIST = 200;

private:
	class MeshComponent* mMeshComponent;
	Actor* mTarget = nullptr;
	Actor* mLaserHolder;
	class LaserComponent* mLaserComponent;
	TurretState mTurretState = TurretState::Idle;
	float mStateTimer = 0.0f;
	Vector3 mFallVelocity;
	float mCountdown = 0.0f;
	float mFireTimer = 0.05f;
	float mSearchTimer = 0.0f;
	float mTurnTime = 0.0f;
	bool mFirstRound = false;
	bool mSecondRound = false;
	bool mFirstDamage = true;
	int mSoundHandle = 0;
	Vector3 mRandomPoint;
	Quaternion mAimQuat;
	Quaternion mCurrQuat = Quaternion::Identity;
	std::unordered_map<TurretState, std::string> mStateSounds;
};