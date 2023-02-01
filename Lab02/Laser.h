#include "Actor.h"

class Laser : public Actor
{
public:
	Laser(class Game* game);
	void OnUpdate(float deltaTime) override;

	//Getters
	float GetLifetime() const { return mLifetime; }

	//Constants
	static const int LASER_EFFECTIVE_DISTANCE = 70;
	const float MAX_LASER_LIFETIME = 1.0f;

protected:
	class Game* mGame;
	float mLifetime = 0.0f;

	//Components
	class SpriteComponent* mSpriteComponent;
	class MoveComponent* mMoveComponent;
};