#include "MoveComponent.h"
#include "Math.h"

class GoombaMove : public MoveComponent
{
public:
	GoombaMove(class Goomba* owner);
	void Update(float deltaTime) override;
	const float GRAVITY = 2000.0f;

protected:
	float mYSpeed = 0.0f;
	Game* mGame = nullptr;
	class Goomba* mOwner = nullptr;
};