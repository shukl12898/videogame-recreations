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
	static const int SWORD_UP_OFFSET = -40;
	static const int SWORD_DOWN_OFFSET = 40;
	static const int SWORD_LEFT_OFFSET = -32;
	static const int SWORD_RIGHT_OFFSET = 32;
	static const int SWORD_VERTICAL_WIDTH = 20;
	static const int SWORD_VERTICAL_HEIGHT = 28;
	static const int SWORD_HORIZONTAL_WIDTH = 28;
	static const int SWORD_HORIZONTAL_HEIGHT = 20;
	const Vector2 CAMERA_OFFSET = Vector2(-256.0f, -224.0f);

protected:
	const std::string DetermineAnimation() const;
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