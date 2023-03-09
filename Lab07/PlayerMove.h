#pragma once
#include "MoveComponent.h"
#include "Math.h"

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Actor* owner);
	const Vector3& GetVelocity() { return mVelocity; };
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	int GetShieldLevel() const { return mShieldLevel; };

	static const int SHIP_SPEED = 300;

protected:
	Vector3 mVelocity;
	int mBlocksEnd = 0;
	int mBlockNum = 0;
	int mObstaclesEnd = 1000;
	int mObstacleNum = 0;
	bool mLastFrameSpace = false;
	bool mLastFrameQ = false;
	bool mDamage = false;
	bool mRoll = false;
	bool mDamageSound = false;
	int mDamageHandle = 0;
	float mDamageTime = 1.0f;
	float mRollTimer = 0.5f;
	int mShieldLevel = 3;
	float mSpeedMultiplier = 1.0f;
	float mSpeedTimer = 0.0f;
};