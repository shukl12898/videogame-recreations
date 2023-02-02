#include "Actor.h"
#include "Math.h"
#include <unordered_map>

class Frog : public Actor
{
public:
	Frog(class Game* game);
	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;
	void SetInitialPosition(Vector2 update) { mInitialPosition = update; }

protected:
	class SpriteComponent* mSpriteComponent;
	Vector2 mInitialPosition;
	std::unordered_map<int, bool> mLastKeyFrame;
	class CollisionComponent* mCollisionComponent;
	class Game* mGame;
	bool mRiding = false;
};