#include "MoveComponent.h"
#include "Math.h"

class WrappingMove : public MoveComponent
{
public:
	WrappingMove(class Actor* owner);
	void Update(float deltaTime) override;
	void SetMoveDirection(Vector2 direction) { mMoveDirection = direction; }
	Vector2 GetMoveDirection() { return mMoveDirection; }

protected:
	Vector2 mMoveDirection;
};