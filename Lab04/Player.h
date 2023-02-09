#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class SpriteComponent* mSpriteComponent;
	class PlayerMove* mPlayerMove;
};