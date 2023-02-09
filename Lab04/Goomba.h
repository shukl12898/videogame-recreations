#include "Actor.h"

class Goomba : public Actor
{
public:
	Goomba(class Game* game);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };
	class AnimatedSprite* GetAnimatedSprite() { return mASC; };
	void SetStomped() { stomped = true; };
	bool GetStomped() { return stomped; };
	~Goomba();

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class AnimatedSprite* mASC;
	class GoombaMove* mGoombaMove;
	bool stomped = false;
};