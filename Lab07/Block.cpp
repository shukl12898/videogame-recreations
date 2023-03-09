#include "Block.h"
#include "Actor.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Player.h"

Block::Block(Game* game, size_t textureIndex)
: Actor(game)
{
	mGame = game;
	SetScale(25.0f);
	mMeshComponent = new MeshComponent(this);
	mMeshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mMeshComponent->SetTextureIndex(textureIndex);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(1, 1, 1);
	mGame->GetBlocks().push_back(this);

	if (textureIndex == 4)
	{
		mIsExploding = true;
	}
}

Block::~Block()
{
	auto toDelete = std::find(mGame->GetBlocks().begin(), mGame->GetBlocks().end(), this);
	mGame->GetBlocks().erase(toDelete);

	for (Component* i : mComponents)
	{
		delete i;
	}
	mComponents.clear();
}

void Block::OnUpdate(float deltaTime)
{
	if (mGame->GetPlayer()->GetPosition().x - GetPosition().x > 2000)
	{
		SetState(ActorState::Destroy);
	}
}