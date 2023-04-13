#include "Block.h"
#include "MeshComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"

Block::Block(class Game* game)
: Actor(game)
{
	SetScale(64);
	mMeshComponent = new MeshComponent(this);
	mMeshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Meshes/Cube.gpmesh"));
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(1.0f, 1.0f, 1.0f);
	mGame->AddCollider(this);
}

Block::~Block()
{
	mGame->RemoveCollider(this);
}