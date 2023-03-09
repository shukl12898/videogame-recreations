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
				mGame->GetAudio()->PlaySound("BlockExplode.wav", false);
				Explosion(blocks[i]);
			}

			SetState(ActorState::Destroy);
		}
	}
}

void Bullet::Explosion(Block* block)
{

	std::vector<Block*> inRadius;

	for (int i = 0; i < mGame->GetBlocks().size(); i++)
	{
		float distance = block->GetPosition().Distance(block->GetPosition(),
													   mGame->GetBlocks()[i]->GetPosition());
		if (distance <= 50 && mGame->GetBlocks()[i]->GetState() == ActorState::Active)
		{
			inRadius.push_back(mGame->GetBlocks()[i]);
		}
	}

	for (int i = 0; i < inRadius.size(); i++)
	{
		inRadius[i]->SetState(ActorState::Destroy);
		if (inRadius[i]->GetIsExploding())
		{
			Explosion(inRadius[i]);
		}
	}
}