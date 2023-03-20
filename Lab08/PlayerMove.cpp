#include "PlayerMove.h"
#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Random.h"
#include "CollisionComponent.h"
#include "Player.h"

PlayerMove::PlayerMove(Actor* owner)
: MoveComponent(owner)
{
	mOwner = owner;
	Vector3 velocity(400.0f, 0.0f, 0.0f);
	mVelocity = velocity;
}

void PlayerMove::Update(float deltaTime)
{
	Vector3 eye = mOwner->GetPosition() - (mOwner->GetForward() * HDIST) + (Vector3::UnitZ * VDIST);
	Vector3 target = mOwner->GetPosition() + (mOwner->GetForward() * TARGET_DIST);

	Matrix4 view = Matrix4::CreateLookAt(eye, target, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(view);

}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
}