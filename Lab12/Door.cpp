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
	mLeft = new Actor(mGame, this);
	MeshComponent* meshComp2 = new MeshComponent(mLeft);
	Mesh* mesh2 = game->GetRenderer()->GetMesh("Assets/Meshes/DoorLeft.gpmesh");
	meshComp2->SetMesh(mesh2);
	mRight = new Actor(mGame, this);
	MeshComponent* meshComp3 = new MeshComponent(mRight);
	Mesh* mesh3 = game->GetRenderer()->GetMesh("Assets/Meshes/DoorRight.gpmesh");
	meshComp3->SetMesh(mesh3);
	mGame->AddCollider(this);
	mGame->AddDoor(this, mName);
}

Door::~Door()
{
	mGame->RemoveCollider(this);
}

void Door::OnUpdate(float deltaTime)
{
	if (mOpen)
	{
		mOpenTime += deltaTime;
		float time = Math::Clamp<float>(mOpenTime, 0, 1);
		mLeft->SetPosition(Vector3::Lerp(Vector3::Zero, Vector3(0, -100, 0), time));
		mRight->SetPosition(Vector3::Lerp(Vector3::Zero, Vector3(0, 100, 0), time));
		mGame->RemoveCollider(this);
	}
}