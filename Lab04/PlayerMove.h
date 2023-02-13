#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include <string>

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Player* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	const float GRAVITY = 2000.0f;
	const float NORMAL_JUMP = 700.0f;
	const float HALF_JUMP = 350.0f;
	const float MARIO_SPEED = 300.0f;
	const int WIN_LOCATION = 6368;
	const int IN_DITCH = 448;
	const int FALL_THROUGH = 500;

protected:
	std::string DetermineAnimation(bool jump);
	float mYSpeed = 0.0f;
	Game* mGame = nullptr;
	class Player* mOwner = nullptr;
	bool mSpacePressed = false;
	bool mInAir = false;
};