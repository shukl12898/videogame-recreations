#include "Actor.h"

class Frog : public Actor
{
public:
	Frog(class Game* game);
	void SetTexture(char classification);

protected:
	class SpriteComponent* mSpriteComponent;
	class Game* mGame;
};