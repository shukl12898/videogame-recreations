#include "Bullet.h"
#include "Actor.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "Block.h"

Bullet::Bullet(Game* game)
: Actor(game)
{
	mGame = game;
	SetScale(5.0f);
	mMeshComponent = new MeshComponent(this);
	mMeshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(10, 10, 10);
	mMoveComponent = new MoveComponent(this);
	mMoveComponent->SetForwardSpeed(900.0f);
}

Bullet::~Bullet()
{
}

void Bullet::OnUpdate(float deltaTime)
{
	mLifetime -= deltaTime;

	if (mLifetime <= 0)
	{
		SetState(ActorState::Destroy);
	}

	std::vector<Block*> blocks = mGame->GetBlocks();

	for (int i = 0; i < blocks.size(); i++)
	{
		if (mCollisionComponent->Intersect(blocks[i]->GetComponent<CollisionComponent>()))
		{
			if (blocks[i]->GetIsExploding())
			{
				blocks[i]->SetState(ActorState::Destroy);
				Explosion();
			}

			SetState(ActorState::Destroy);
		}
	}
}

void Bullet::Explosion()
{
	std::vector<Block*> blocks = mGame->GetBlocks();

	for (int i = 0; i < blocks.size(); i++)
	{
		float distance = mPosition.Distance(mPosition, blocks[i]->GetPosition());
		if (distance < 50)
		{
			if (blocks[i]->GetIsExploding())
			{
				blocks[i]->SetState(ActorState::Destroy);
				//figure out recursion thing here
				// Explosion();
			}
		}
	}
}