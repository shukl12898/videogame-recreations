#include "Actor.h"
#include "Prop.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include <string>

Prop::Prop(std::string mesh, bool usesAlpha, bool hasCollisions, Game* game):Actor(game)
{
	mMeshComponent = new MeshComponent(this, usesAlpha);
	Mesh* currMesh = mGame->GetRenderer()->GetMesh(mesh);
	mMeshComponent->SetMesh(currMesh);
	if (hasCollisions)
	{
		mCollisionComponent = new CollisionComponent(this);
		mCollisionComponent->SetSize(currMesh->GetWidth(), currMesh->GetHeight(),
									 currMesh->GetDepth());
		mGame->AddCollider(this);
	}
}

Prop::~Prop()
{
	mGame->RemoveCollider(this);
}