#include "Actor.h"

class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);
	~Asteroid();
	void OnUpdate(float deltaTime) override;

protected:
	class Game* mGame;
	class SpriteComponent* mSpriteComponent;
	class MoveComponent* mMoveComponent;
};