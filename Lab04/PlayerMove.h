#include "MoveComponent.h"
#include "Math.h"
#include <string>

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Player* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;
	// void SetMoveDirection(Vector2 direction) { mMoveDirection = direction; }
	// Vector2 GetMoveDirection() { return mMoveDirection; }
	const float GRAVITY = 2000.0f;

protected:
	std::string DetermineAnimation(bool jump);
	float mYSpeed = 0.0f;
	Game* mGame = nullptr;
	class Player* mOwner = nullptr;
	bool mSpacePressed = false;
	bool mInAir = false;
	// Vector2 mMoveDirection;
};