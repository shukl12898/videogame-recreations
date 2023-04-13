#include "Actor.h"
#include "EnergyCatcher.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"

EnergyCatcher::EnergyCatcher(Game* game)
: Actor(game)
{
	mGame = game;
	mMeshComponent = new MeshComponent(this);
	Mesh* mesh = game->GetRenderer()->GetMesh("Assets/Meshes/EnergyCatcher.gpmesh");
	mMeshComponent->SetMesh(mesh);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(50.0f, 50.0f, 50.0f);
	mGame->AddCollider(this);
}