#include "Actor.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);
	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;

	//Setters and Getters
	float GetCooldown() const { return mCooldown; }
	void SetCooldown(float cooldown) { mCooldown = cooldown; }

	//Constants
	const float SHIP_FORWARD_SPEED = 150.0;
	const float SHIP_ANGULAR_SPEED = Math::PiOver2;

protected:
	float mCooldown = 0.0f;
	class Game* mGame;

	//Components
	class MoveComponent* mMoveComponent;
	class SpriteComponent* mSpriteComponent;
};