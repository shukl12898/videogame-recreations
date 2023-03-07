#include "SideBlock.h"
#include "Actor.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"

SideBlock::SideBlock(Game* game, size_t textureIndex)
: Actor(game)
{
	mGame = game;
	SetScale(500.0f);
	mMeshComponent = new MeshComponent(this);
	mMeshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mMeshComponent->SetTextureIndex(textureIndex);
}

void SideBlock::OnUpdate(float deltaTime)
{
	if (mGame->GetPlayer()->GetPosition().x - GetPosition().x > 2000)
	{
		SetState(ActorState::Destroy);
	}
}
