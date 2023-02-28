#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include <string>

class PlayerMove : public MoveComponent
{
public:
	enum DirectionState
	{
		Up,
		Down,
		Left,
		Right
	};

	PlayerMove(class Player* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	static const int MOVE_SPEED = 150;

protected:
	std::string DetermineAnimation();
	Vector2 mDirection;
	Game* mGame = nullptr;
	bool mMoving = false;
	int mDirectionState = DirectionState::Down;
	class Player* mOwner = nullptr;
	bool mSpacePressed = false;
};