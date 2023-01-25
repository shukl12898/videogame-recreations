#include "Actor.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);
	void OnProcessInput(const Uint8* keyState);

protected:
	float mForwardSpeed = 150;
	float mAngularSpeed = Math::PiOver2;
	class MoveComponent* mMoveComponent;
	class SpriteComponent* mSpriteComponent;
	class Game* mGame;
};