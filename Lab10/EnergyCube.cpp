#include "Actor.h"
#include "EnergyCube.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"

EnergyCube::EnergyCube(Game* game)
: Actor(game)
{
	mGame = game;
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(25.0f, 25.0f, 25.0f);
	Mesh* mesh = mGame->GetRenderer()->GetMesh("Assets/Meshes/EnergyCube.gpmesh");
	mMeshComponent = new MeshComponent(this);
	mMeshComponent->SetMesh(mesh);
	mGame->AddCollider(this);
}