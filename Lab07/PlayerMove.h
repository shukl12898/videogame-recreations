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

	static const int SHIP_SPEED = 300;

protected:
	Vector3 mVelocity;
	int mBlocksEnd = 0;
};