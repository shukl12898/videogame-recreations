#include "Actor.h"

class Log : public Actor
{
public:
	Log(class Game* game);
	void SetTexture(char classification);

protected:
	class SpriteComponent* mSpriteComponent;
	class Game* mGame;
};