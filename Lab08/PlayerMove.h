#pragma once
#include "VehicleMove.h"
#include "Math.h"

class PlayerMove : public VehicleMove
{
public:
	PlayerMove(class Actor* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	void OnLapChange(int newLap) override;

	static const int SHIP_SPEED = 300;

private:
	int mSound = 0;
};