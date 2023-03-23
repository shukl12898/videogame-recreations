#pragma once
#include "Component.h"
#include "Math.h"
#include <vector>

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
	virtual void OnLapChange(int newLap) {}
	void Update(float deltaTime) override;
	void SetPedal(bool value) { mAcceleratorPressed = value; };
	void SetTurn(int value) { mTurnDirection = value; };
	int GetLap() { return mCurrLap; };
	int GetCheck() { return mLastCheck; };
	int GetDistCheck();

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
	class HeightMap* mHeightMap = nullptr;
	Vector3 mVelocity = Vector3::Zero;
	std::vector<std::vector<Vector2>> mCheckpoints;
	int mCurrLap = 0;
	int mLastCheck = -1;
	bool mStart = true;
};