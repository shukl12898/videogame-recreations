#include "PlayerMove.h"
#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Random.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "Player.h"

PlayerMove::PlayerMove(Actor* owner)
: MoveComponent(owner)
{
	mOwner = owner;
	ChangeState(Falling);
	mGravity = Vector3(0.0f, 0.0f, -980.0f);
}

void PlayerMove::Update(float deltaTime)
{
	switch (mCurrentState)
	{
	case OnGround:
		UpdateOnGround(deltaTime);
		break;
	case Jump:
		UpdateJump(deltaTime);
		break;
	case Falling:
		UpdateFalling(deltaTime);
		break;
	}
}

void PlayerMove::PhysicsUpdate(float deltaTime)
{
	mAcceleration = mPendingForces * (1.0f / mMass);
	mVelocity += mAcceleration * deltaTime;
	FixXYVelocity();
	mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);
	mOwner->SetRotation(mOwner->GetRotation() + mAngularSpeed * deltaTime);
	mPendingForces = Vector3::Zero;
}

void PlayerMove::UpdateOnGround(float deltaTime)
{
	PhysicsUpdate(deltaTime);
}

void PlayerMove::UpdateJump(float deltaTime)
{
	PhysicsUpdate(deltaTime);
}

void PlayerMove::UpdateFalling(float deltaTime)
{
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
}

void PlayerMove::FixXYVelocity()
{
	Vector2 xyVelocity(mVelocity.x, mVelocity.y);
	if (xyVelocity.Length() > MAX_SPEED)
	{
		xyVelocity = xyVelocity.Normalize(xyVelocity);
		xyVelocity = MAX_SPEED * xyVelocity;
	}

	if (mCurrentState == OnGround)
	{
		if ((std::abs(mAcceleration.x) < 0.1) || (mAcceleration.x > 0 && xyVelocity.x < 0) ||
			(mAcceleration.x < 0 && xyVelocity.x > 0))
		{
			xyVelocity.x *= 0.9f;
		}

		if ((std::abs(mAcceleration.y) < 0.1) || (mAcceleration.y > 0 && xyVelocity.y < 0) ||
			(mAcceleration.y < 0 && xyVelocity.y > 0))
		{
			xyVelocity.y *= 0.9f;
		}
	}

	mVelocity.x = xyVelocity.x;
	mVelocity.y = xyVelocity.y;
}

void PlayerMove::ProcessInput(const Uint8* keyState, Uint32 mouseButtons,
							  const Vector2& relativeMouse)
{

	//w sets adds force in owner's forward
	if (keyState[SDL_SCANCODE_W])
	{
		AddForce(mOwner->GetForward() * 700.0f);
	}
	//s adds force in opposite direction to forward
	if (keyState[SDL_SCANCODE_S])
	{
		AddForce(mOwner->GetForward() * -700.0f);
	}
	//d adds force in owner's right
	if (keyState[SDL_SCANCODE_D])
	{
		AddForce(mOwner->GetRight() * 700.0f);
	}
	//a adds force in opposite direction to right
	if (keyState[SDL_SCANCODE_A])
	{
		AddForce(mOwner->GetRight() * -700.0f);
	}

	float angularSpeed = relativeMouse.x / 500.0f * Math::Pi * 10.0f;
	float pitchSpeed = relativeMouse.y / 500.0f * Math::Pi * 10.0f;
	SetAngularSpeed(angularSpeed);
	mOwner->GetComponent<CameraComponent>()->SetPitchSpeed(pitchSpeed);
}