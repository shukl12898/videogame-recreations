#include "CameraComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor* owner)
: Component(owner)
{
}

void CameraComponent::Update(float deltaTime)
{
	Vector3 eye = mOwner->GetPosition() - (mOwner->GetForward() * HDIST) + (Vector3::UnitZ * VDIST);
	Vector3 target = mOwner->GetPosition() + (mOwner->GetForward() * TARGET_DIST);

	Matrix4 view = Matrix4::CreateLookAt(eye, target, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(view);
}