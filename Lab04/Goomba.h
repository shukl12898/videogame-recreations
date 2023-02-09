#include "Actor.h"

class Goomba : public Actor
{
public:
	Goomba(class Game* game);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };
	class AnimatedSprite* GetAnimatedSprite() { return mASC; };
	void SetStomped() { mStomped = true; };
	bool GetStomped() const { return mStomped; };
	void OnUpdate(float deltaTime) override;

	~Goomba();

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	class AnimatedSprite* mASC;
	float mDeathTime = 0.0f;
	class GoombaMove* mGoombaMove;
	bool mStomped = false;
};