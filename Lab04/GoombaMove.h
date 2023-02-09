#include "MoveComponent.h"
#include "Math.h"

class GoombaMove : public MoveComponent
{
public:
	GoombaMove(class Goomba* owner);
	void Update(float deltaTime) override;
	// void ProcessInput(const Uint8* keyState) override;
	const float GRAVITY = 2000.0f;

protected:
	float mYSpeed = 0.0f;
	float deathTime = 0.0f;
	Game* mGame = nullptr;
	class Goomba* mOwner = nullptr;
	// bool mSpacePressed = false;
	// bool mInAir = false;
	// Vector2 mMoveDirection;
};