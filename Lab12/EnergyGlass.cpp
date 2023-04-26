#include "Actor.h"
#include "EnergyGlass.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"

EnergyGlass::EnergyGlass(Game* game)
: Actor(game)
{
	mGame = game;
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(1.0f, 1.0f, 1.0f);
	mGame->AddCollider(this);
	Mesh* mesh = mGame->GetRenderer()->GetMesh("Assets/Meshes/Cube.gpmesh");
	mMeshComponent = new MeshComponent(this, true);
	mMeshComponent->SetMesh(mesh);
	mMeshComponent->SetTextureIndex(17);
}

EnergyGlass::~EnergyGlass()
{
	mGame->RemoveCollider(this);
}