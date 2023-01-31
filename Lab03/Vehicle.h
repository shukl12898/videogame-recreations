#include "Actor.h"

class Vehicle : public Actor
{
public:
	Vehicle(class Game* game);
	class SpriteComponent* mSpriteComponent;
	void SetTexture(char classification);
	class WrappingMove* GetWrappingMove() { return mWrappingMove; };

protected:
	class Game* mGame;
	class WrappingMove* mWrappingMove;
};