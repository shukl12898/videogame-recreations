#include "Actor.h"
#include "EnergyCatcher.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include <unordered_map>
#include "Door.h"

EnergyCatcher::EnergyCatcher(Game* game, std::string doorName)
: Actor(game)
{
	mGame = game;
	mDoorName = doorName;
	mMeshComponent = new MeshComponent(this);
	Mesh* mesh = game->GetRenderer()->GetMesh("Assets/Meshes/EnergyCatcher.gpmesh");
	mMeshComponent->SetMesh(mesh);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(50.0f, 50.0f, 50.0f);
	mGame->AddCollider(this);
}

EnergyCatcher::~EnergyCatcher()
{
	mGame->RemoveCollider(this);
}

void EnergyCatcher::Activate()
{
	mActivated = true;
	mGame->GetAudio()->PlaySound("EnergyCaught.ogg", false, this);
	std::unordered_map<std::string, Door*> doorNames = mGame->GetDoorNames();
	if (doorNames.find(mDoorName) != doorNames.end())
	{
		mGame->RemoveCollider(doorNames[mDoorName]);
		doorNames[mDoorName]->OpenDoor();
	}
}