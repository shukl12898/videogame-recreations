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
}

void PlayerMove::Update(float deltaTime)
{
	MoveComponent::Update(deltaTime);
}

void PlayerMove::ProcessInput(const Uint8* keyState, Uint32 mouseButtons,
							  const Vector2& relativeMouse)
{

	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;

	//w sets forward speed to 350.0f
	if ((keyState[SDL_SCANCODE_W]) && (!keyState[SDL_SCANCODE_S]))
	{
		forwardSpeed = 350.0f;
	}
	//s sets forward speed to -350.0f
	else if (!(keyState[SDL_SCANCODE_W]) && (keyState[SDL_SCANCODE_S]))
	{
		forwardSpeed = -350.0f;
	}

	//d sets forward speed to 350.0f
	if ((keyState[SDL_SCANCODE_D]) && (!keyState[SDL_SCANCODE_A]))
	{
		strafeSpeed = 350.0f;
	}
	//a sets forward speed to -350.0f
	else if (!(keyState[SDL_SCANCODE_D]) && (keyState[SDL_SCANCODE_A]))
	{
		strafeSpeed = -350.0f;
	}

	SetForwardSpeed(forwardSpeed);
	SetStrafeSpeed(strafeSpeed);

	float angularSpeed = relativeMouse.x / 500.0f * Math::Pi * 10.0f;
	float pitchSpeed = relativeMouse.y / 500.0f * Math::Pi * 10.0f;
	SetAngularSpeed(angularSpeed);
	mOwner->GetComponent<CameraComponent>()->SetPitchSpeed(pitchSpeed);
}