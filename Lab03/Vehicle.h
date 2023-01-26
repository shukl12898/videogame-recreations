#include "Actor.h"

class Vehicle : public Actor
{
public:
	Vehicle(class Game* game);
	void SetTexture(char classification);

protected:
	class SpriteComponent* mSpriteComponent;
	class Game* mGame;
};