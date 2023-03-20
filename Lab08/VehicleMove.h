#pragma once
#include "Component.h"
#include "Math.h"

class VehicleMove : public Component
{
public:
	enum Turn
	{
		None,
		Left,
		Right
	};

	VehicleMove(class Actor* owner);
	void Update(float deltaTime) override;
	void SetPedal(bool value) { mAcceleratorPressed = value; };
	void SetTurn(int value) { mTurnDirection = value; };

	const float MIN_ACCELERATION = 1000.0f;
	const float MAX_ACCELERATION = 2500.0f;
	const float RAMP_TIME = 2.0f;
	const float ANGULAR_ACCELERATION = 5.0 * Math::Pi;
	const float LINEAR_DRAG_ACC = 0.9f;
	const float LINEAR_DRAG_NOACC = 0.975f;
	const float ANGULAR_DRAG = 0.9f;

private:
	bool mAcceleratorPressed = false;
	int mTurnDirection = Turn::None;
	float mAccelerationTime = RAMP_TIME;
	float mAccelerationMag = 1000.0f;
	float mAngularVelocity = 0.0f;
	Vector3 mVelocity = Vector3::Zero;
};