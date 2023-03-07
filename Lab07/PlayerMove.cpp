#include "PlayerMove.h"
#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"

PlayerMove::PlayerMove(Actor* owner)
: MoveComponent(owner)
{
	mOwner = owner;
	Vector3 velocity(400.0f, 0.0f, 0.0f);
	mVelocity = velocity;
}

void PlayerMove::Update(float deltaTime)
{

	Vector3 position = mOwner->GetPosition() + (mVelocity * deltaTime);
	position.y = Math::Clamp<float>(position.y, -180, 180);
	position.z = Math::Clamp<float>(position.z, -225, 225);

	mOwner->SetPosition(position);

	Vector3 cameraPos = mOwner->GetPosition() - (mOwner->GetPosition() * 300);
	Vector3 targetPos = mOwner->GetPosition() + (mOwner->GetForward() * 20);
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{

	float forwardSpeed = 0.0f;
	float sideSpeed = 0.0f;

	if (keyState[SDL_SCANCODE_W])
	{
		forwardSpeed += SHIP_SPEED;
	}
	if (keyState[SDL_SCANCODE_S])
	{
		forwardSpeed -= SHIP_SPEED;
	}

	if (keyState[SDL_SCANCODE_A])
	{
		sideSpeed -= SHIP_SPEED;
	}
	if (keyState[SDL_SCANCODE_D])
	{
		sideSpeed += SHIP_SPEED;
	}

	Vector3 velocity(0.0f, sideSpeed, forwardSpeed);
	mVelocity = velocity;
}