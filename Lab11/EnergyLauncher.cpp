#include "Actor.h"
#include "EnergyLauncher.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Pellet.h"
#include <unordered_map>
#include "Door.h"

EnergyLauncher::EnergyLauncher(Game* game)
: Actor(game)
{
	mGame = game;
	mMeshComponent = new MeshComponent(this);
	Mesh* mesh = game->GetRenderer()->GetMesh("Assets/Meshes/EnergyLauncher.gpmesh");
	mMeshComponent->SetMesh(mesh);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(50.0f, 50.0f, 50.0f);
	mGame->AddCollider(this);
}

EnergyLauncher::~EnergyLauncher()
{
	mGame->RemoveCollider(this);
}

void EnergyLauncher::OnUpdate(float deltaTime)
{

	std::unordered_map<std::string, Door*> doorNames = mGame->GetDoorNames();

	if (doorNames.find(mDoorName) != doorNames.end())
	{
		if (doorNames[mDoorName]->GetOpen())
		{
			mLaunch = false;
		}
	}

	if (mLaunch)
	{
		pelletTimer += deltaTime;

		if (pelletTimer >= mCooldown)
		{
			pelletTimer = 0.0f;
			Pellet* pellet = new Pellet(mGame);

			//pellet 20 units in front of it
			Vector3 direction = GetForward();
			Vector3 pelletPos = GetPosition() + 20 * direction;
			pellet->SetPosition(pelletPos);
			//call calcworld transform on pellet
			pellet->CalcWorldTransform();
			//pellet moving in forward direction of energy launcher (get forward)
			pellet->SetForward(GetForward());
		}
	}
}