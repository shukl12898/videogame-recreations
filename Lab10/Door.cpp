#include "Actor.h"
#include "Door.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"

Door::Door(Game* game, std::string name)
: Actor(game)
{
	mGame = game;
	mName = name;
	mMeshComponent = new MeshComponent(this);
	Mesh* mesh = game->GetRenderer()->GetMesh("Assets/Meshes/DoorFrame.gpmesh");
	mMeshComponent->SetMesh(mesh);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(mesh->GetWidth(), mesh->GetHeight(), mesh->GetDepth());
	mGame->AddCollider(this);
	mGame->AddDoor(this, mName);
}