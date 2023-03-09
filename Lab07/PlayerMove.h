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
	void Explosion();

	static const int SHIP_SPEED = 300;

protected:
	Vector3 mVelocity;
	int mBlocksEnd = 0;
	int mBlockNum = 0;
	int mObstaclesEnd = 1000;
	int mObstacleNum = 0;
	bool mLastFrame = false;
	bool mDamage = false;
	float mDamageTime = 1.0f;
	int mShieldLevel = 3;
};