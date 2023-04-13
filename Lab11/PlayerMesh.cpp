#include "Actor.h"
#include "MeshComponent.h"
#include "PlayerMesh.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "CameraComponent.h"

PlayerMesh::PlayerMesh(Game* game)
: Actor(game)
{
	mMeshComponent = new MeshComponent(this);
	mMeshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Meshes/PortalGun.gpmesh"));
	SetScale(Vector3(1.0f, 2.5f, 2.5f));
}

void PlayerMesh::OnUpdate(float deltaTime)
{
	Vector3 unproject = mGame->GetRenderer()->Unproject(Vector3(300.0f, -250.0f, 0.4f));
	SetPosition(unproject);
	float pitchAngle = mGame->GetPlayer()->GetComponent<CameraComponent>()->GetPitchAngle();
	float yawAngle = mGame->GetPlayer()->GetRotation();
	Quaternion pitch = Quaternion(Vector3(0, 1, 0), pitchAngle);
	Quaternion yaw = Quaternion(Vector3(0, 0, 1), yawAngle);

	Quaternion final = Quaternion::Concatenate(pitch, yaw);
	SetQuat(final);
}