#include "PortalGun.h"
#include "Actor.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Player.h"

PortalGun::PortalGun(Game* game)
: Actor(game)
{
	mMeshComponent = new MeshComponent(this);
	mMeshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Meshes/PortalGun.gpmesh"));
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(8.0f, 8.0f, 8.0f);
}

void PortalGun::OnUpdate(float deltaTime)
{
	mRotation += deltaTime * ROTATION_RATE;
	if (mCollisionComponent->Intersect(mGame->GetPlayer()->GetComponent<CollisionComponent>()))
	{
		SetState(ActorState::Destroy);
		mGame->GetPlayer()->GiveGun();
	}
}