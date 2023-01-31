#include "Actor.h"

class Log : public Actor
{
public:
	Log(class Game* game);
	class SpriteComponent* mSpriteComponent;
	class WrappingMove* GetWrappingMove() { return mWrappingMove; };

protected:
	class Game* mGame;
	class WrappingMove* mWrappingMove;
};