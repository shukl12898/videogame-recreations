#include "Actor.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);
	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;
	float GetCooldown() const { return mCooldown; }
	void SetCooldown(float cooldown) { mCooldown = cooldown; }

protected:
	float mForwardSpeed = 150;
	float mCooldown = 0.0f;
	float mAngularSpeed = Math::PiOver2;
	class MoveComponent* mMoveComponent;
	class SpriteComponent* mSpriteComponent;
	class Game* mGame;
};