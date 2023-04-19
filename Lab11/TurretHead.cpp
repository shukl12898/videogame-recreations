#include "TurretHead.h"
#include "Mesh.h"
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "MeshComponent.h"

TurretHead::TurretHead(Game* game, Actor* parent)
: Actor(game, parent)
{
	mGame = game;
	SetScale(0.75f);
	mPosition = Vector3(0.0f, 0.0f, 18.75f);
	mMeshComponent = new MeshComponent(this);
	Mesh* mesh = mGame->GetRenderer()->GetMesh("Assets/Meshes/TurretHead.gpmesh");
	mMeshComponent->SetMesh(mesh);
}

void TurretHead::OnUpdate(float deltaTime)
{
	mRotation += Math::TwoPi * deltaTime;
}