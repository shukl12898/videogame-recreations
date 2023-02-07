#include "Actor.h"

class Block : public Actor
{
public:
	Block(class Game* game, char classification);
	~Block();

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class SpriteComponent* mSpriteComponent;
};