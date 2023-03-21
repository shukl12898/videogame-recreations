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

	//Calculate an 'ideal position' for camera
	Vector3 idealPosition = Ideal();

	//Numeric integration of spring
	Vector3 displacement = mCameraPos - idealPosition;
	Vector3 springAcc = (-SPRING * displacement) - (DAMP * mCameraVelocity);
	mCameraVelocity += springAcc * deltaTime;
	mCameraPos += mCameraVelocity * deltaTime;

	Vector3 target = mOwner->GetPosition() + (mOwner->GetForward() * TARGET_DIST);

	Matrix4 view = Matrix4::CreateLookAt(mCameraPos, target, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(view);
}

Vector3 CameraComponent::Ideal()
{
	Vector3 eye = mOwner->GetPosition() - (mOwner->GetForward() * HDIST) + (Vector3::UnitZ * VDIST);
	eye.z = 70.0f;
	return eye;
}

void CameraComponent::SnapToIdeal()
{
	Vector3 eye = mOwner->GetPosition() - (mOwner->GetForward() * HDIST) + (Vector3::UnitZ * VDIST);
	eye.z = 70.0f;
	mCameraPos = eye;
	Vector3 target = mOwner->GetPosition() + (mOwner->GetForward() * TARGET_DIST);
	Matrix4 view = Matrix4::CreateLookAt(eye, target, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(view);
}