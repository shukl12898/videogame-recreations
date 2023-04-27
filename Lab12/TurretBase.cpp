#include "TurretBase.h"
#include "TurretHead.h"
#include "Mesh.h"
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"

TurretBase::TurretBase(Game* game)
: Actor(game)
{
	mGame = game;
	SetScale(0.75f);
	mPosition = Vector3(0.0f, 0.0f, 18.75f);
	mMeshComponent = new MeshComponent(this);
	Mesh* mesh = mGame->GetRenderer()->GetMesh("Assets/Meshes/TurretBase.gpmesh");
	mMeshComponent->SetMesh(mesh);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(25.0f, 110.0f, 25.0f);
	mHead = new TurretHead(game, this);
	mHealthComponent = new HealthComponent(this);
	mHealthComponent->SetOnDeath([this] {
		Die();
	});
	mHealthComponent->SetOnDamage([this](const Vector3&) {
		mHead->TakeDamage();
	});
	mGame->AddCollider(this);
}

TurretBase::~TurretBase()
{
	mGame->RemoveCollider(this);
}

void TurretBase::Die()
{
	mHead->Die();
}