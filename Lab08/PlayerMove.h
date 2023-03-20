#pragma once
#include "VehicleMove.h"
#include "Math.h"

class PlayerMove : public VehicleMove
{
public:
	PlayerMove(class Actor* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;

	static const int SHIP_SPEED = 300;
	static const int HDIST = 60;
	static const int VDIST = 70;
	static const int TARGET_DIST = 50;

};