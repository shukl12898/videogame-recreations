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
	const std::string DetermineAnimation();
	void SwordUpdate();
	Vector2 mDirection;
	Game* mGame = nullptr;
	bool mMoving = false;
	bool mAttack = false;
	bool mLastFrame = false;
	float mAttackTime = 0.0f;
	int mDirectionState = DirectionState::Down;
	class Player* mOwner = nullptr;
	bool mSpacePressed = false;
	class Sword* mSword;
};