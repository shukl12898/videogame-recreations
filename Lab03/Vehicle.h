#include "Actor.h"

class Vehicle : public Actor
{
public:
	Vehicle(class Game* game);
	~Vehicle();
	void SetTexture(char classification);
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; }
	void OnUpdate(float deltaTime) override;
	class WrappingMove* GetWrappingMove() { return mWrappingMove; };
	float const VEHICLE_SPEED = 100.0f;

protected:
	class Game* mGame;
	class SpriteComponent* mSpriteComponent;
	class WrappingMove* mWrappingMove;
	class CollisionComponent* mCollisionComponent;
};