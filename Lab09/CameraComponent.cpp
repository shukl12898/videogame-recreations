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

	mPitchAngle += mPitchSpeed * deltaTime;

	mPitchAngle = Math::Clamp(mPitchAngle, -Math::Pi / 2.1f, Math::Pi / 2.1f);

	Matrix4 pitchRotation = Matrix4::CreateRotationY(mPitchAngle);
	Matrix4 yawRotation = Matrix4::CreateRotationZ(mOwner->GetRotation());
	Matrix4 rotation = pitchRotation * yawRotation;

	Vector3 transform = Vector3::Transform(Vector3::UnitX, rotation);

	Vector3 target = mOwner->GetPosition() + (transform * TARGET_DIST);

	Matrix4 view = Matrix4::CreateLookAt(mOwner->GetPosition(), target, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(view);
}

Vector3 CameraComponent::Ideal()
{
	Vector3 eye = mOwner->GetPosition() - (mOwner->GetForward() * HDIST) + (Vector3::UnitZ * VDIST);
	eye.z = 70.0f;
	return eye;
}
