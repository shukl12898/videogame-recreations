#include "Actor.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);
	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;
	float GetCooldown() const { return mCooldown; }
	void SetCooldown(float cooldown) { mCooldown = cooldown; }
	const float SHIP_FORWARD_SPEED = 150.0;
	const float SHIP_ANGULAR_SPEED = Math::PiOver2;

protected:
	float mCooldown = 0.0f;
	class MoveComponent* mMoveComponent;
	class SpriteComponent* mSpriteComponent;
	class Game* mGame;
};