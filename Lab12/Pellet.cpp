#include "Actor.h"
#include "Pellet.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Player.h"
#include "Portal.h"
#include "EnergyCatcher.h"
#include "EnergyGlass.h"
#include "EnergyCube.h"
#include "HealthComponent.h"

Pellet::Pellet(Game* game)
: Actor(game)
{
	mGame = game;
	mMeshComponent = new MeshComponent(this);
	Mesh* mesh = game->GetRenderer()->GetMesh("Assets/Meshes/Sphere.gpmesh");
	mMeshComponent->SetMesh(mesh);
	mMeshComponent->SetTextureIndex(1);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(25.0f, 25.0f, 25.0f);
}

void Pellet::Teleport(Portal* entry, Portal* exit)
{
	Vector3 direction = exit->GetQuatForward();
	Vector3 position = exit->GetPosition() + 10 * direction;
	SetPosition(position);

	mForward = direction;
	mTeleport = false;
	mTeleportTime = 0.0f;
}

void Pellet::OnUpdate(float deltaTime)
{
	//Pellet should travel 500 units in current movement direction
	SetPosition(GetPosition() + mForward * SPEED * deltaTime);
	mLifetime += deltaTime;
	mTeleportTime += deltaTime;

	if (mCollisionComponent->Intersect(mGame->GetPlayer()->GetComponent<CollisionComponent>()))
	{
		mGame->GetAudio()->PlaySound("PelletDeath.ogg", false, this, false);
		SetState(ActorState::Destroy);
	}

	Portal* blue = mGame->GetBluePortal();
	Portal* orange = mGame->GetOrangePortal();

	if (mTeleport)
	{
		if (blue != nullptr && orange != nullptr)
		{
			//only initiate teleport if you intersect with one of the portals, intersected is entry, opposite is exit
			if (blue->GetComponent<CollisionComponent>()->Intersect(mCollisionComponent))
			{
				Teleport(blue, orange);
			}
			else if (orange->GetComponent<CollisionComponent>()->Intersect(mCollisionComponent))
			{
				Teleport(orange, blue);
			}
		}
	}

	if (mCollisionComponent->Intersect(mGame->GetPlayer()->GetComponent<CollisionComponent>()))
	{
		mGame->GetPlayer()->GetComponent<HealthComponent>()->TakeDamage(100, mPosition);
	}

	if (mLifetime >= 0.25 && mTeleport)
	{
		std::vector<Actor*> colliders = mGame->GetColliders();
		for (int i = 0; i < colliders.size(); i++)
		{
			if (mCollisionComponent->Intersect(colliders[i]->GetComponent<CollisionComponent>()))
			{
				EnergyCatcher* catcher = dynamic_cast<EnergyCatcher*>(colliders[i]);
				EnergyGlass* glass = dynamic_cast<EnergyGlass*>(colliders[i]);
				EnergyCube* cube = dynamic_cast<EnergyCube*>(colliders[i]);
				HealthComponent* healthComponent = colliders[i]->GetComponent<HealthComponent>();

				if (catcher != nullptr)
				{
					Vector3 position = catcher->GetForward() * 40 + catcher->GetPosition();
					SetPosition(position);
					SetState(ActorState::Paused);
					catcher->Activate();
				}

				if (glass != nullptr)
				{
					if (!mGreen)
					{
						mGame->GetAudio()->PlaySound("PelletDeath.ogg", false, this, false);
						SetState(ActorState::Destroy);
					}
				}

				if (cube != nullptr)
				{
					mMeshComponent->SetTextureIndex(2);
					mGreen = true;
				}

				bool skip = true;

				if (healthComponent != nullptr)
				{
					if (!healthComponent->IsDead())
					{
						healthComponent->TakeDamage(100, mPosition);
					}
					else
					{
						skip = false;
					}
				}

				if (catcher == nullptr && glass == nullptr && cube == nullptr && skip)
				{
					mGame->GetAudio()->PlaySound("PelletDeath.ogg", false, this, false);
					SetState(ActorState::Destroy);
				}
			}
		}
	}

	if (!mTeleport && mTeleportTime >= 0.25)
	{
		mTeleport = true;
	}
}