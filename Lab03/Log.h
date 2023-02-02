#include "Actor.h"

class Log : public Actor
{
public:
	Log(class Game* game);
	class SpriteComponent* mSpriteComponent;
	class WrappingMove* GetWrappingMove() { return mWrappingMove; };
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; }

protected:
	class Game* mGame;
	class WrappingMove* mWrappingMove;
	class CollisionComponent* mCollisionComponent;
};