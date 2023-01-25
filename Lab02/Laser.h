#include "Actor.h"

class Laser : public Actor
{
public:
	Laser(class Game* game);
	void OnUpdate(float deltaTime) override;
	float GetLifetime() const { return mLifetime; }

protected:
	class Game* mGame;
	float mLifetime = 0.0f;
	class SpriteComponent* mSpriteComponent;
	class MoveComponent* mMoveComponent;
};