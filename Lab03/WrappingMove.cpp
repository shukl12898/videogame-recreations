#include "WrappingMove.h"
#include "Actor.h"
#include "Game.h"
#include "MoveComponent.h"
#include "Math.h"

WrappingMove::WrappingMove(Actor* owner)
: MoveComponent(owner)
{
	Vector2 initialDirection(1, 0);
	mMoveDirection = initialDirection;
}

void WrappingMove::Update(float deltaTime)
{
	GetOwner()->SetPosition(mOwner->GetPosition() + ((mForwardSpeed * deltaTime) * mMoveDirection));

	float xPos = GetOwner()->GetPosition().x;
	float yPos = GetOwner()->GetPosition().y;
	Game* game = GetGame();

	if (xPos < 0)
	{
		Vector2 wrapPos(static_cast<float>(game->WIDTH), yPos);
		GetOwner()->SetPosition(wrapPos);
	}

	if (xPos > game->WIDTH)
	{
		Vector2 wrapPos(0.0f, yPos);
		GetOwner()->SetPosition(wrapPos);
	}
}