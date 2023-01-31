#include "Actor.h"

class DeadFrog : public Actor
{
public:
	DeadFrog(class Game* game, Vector2 position);
	void OnUpdate(float deltaTime) override;

protected:
	class SpriteComponent* mSpriteComponent;
	float mLifetime = 0.0f;
	class Game* mGame;
};