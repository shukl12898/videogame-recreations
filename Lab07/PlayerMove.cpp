#include "PlayerMove.h"
#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "SideBlock.h"

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

	Game* game = mOwner->GetGame();
	int i = 0;
	while (mBlocksEnd - mOwner->GetPosition().x < 3500)
	{
		SideBlock* block1 = new SideBlock(game, 0);
		block1->SetPosition(Vector3(mBlocksEnd + (i * 500), 500, 0));
		block1->SetRotation(Math::Pi);

		SideBlock* block2 = new SideBlock(game, 0);
		block2->SetPosition(Vector3(mBlocksEnd + (i * 500), -500, 0));

		SideBlock* block3 = new SideBlock(game, 5);
		block3->SetPosition(Vector3(mBlocksEnd + (i * 500), 0, -500));

		SideBlock* block4 = new SideBlock(game, 6);
		block4->SetPosition(Vector3(mBlocksEnd + (i * 500), 0, 500));
		mBlocksEnd += i * 500;
		i++;
	}

	mOwner->SetPosition(position);

	Vector3 eye = mOwner->GetPosition() - (mOwner->GetForward() * 300);
	Vector3 target = mOwner->GetPosition() + (mOwner->GetForward() * 20);

	eye.z = 0;

	Matrix4 view = Matrix4::CreateLookAt(eye, target, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(view);
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

	Vector3 velocity(400.0f, sideSpeed, forwardSpeed);
	mVelocity = velocity;
}