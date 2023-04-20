#include "TurretHead.h"
#include "Mesh.h"
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "LaserComponent.h"

TurretHead::TurretHead(Game* game, Actor* parent)
: Actor(game, parent)
{
	mGame = game;
	SetScale(0.75f);
	mPosition = Vector3(0.0f, 0.0f, 18.75f);
	mMeshComponent = new MeshComponent(this);
	Mesh* mesh = mGame->GetRenderer()->GetMesh("Assets/Meshes/TurretHead.gpmesh");
	mMeshComponent->SetMesh(mesh);
	mLaserHolder = new Actor(game, this);
	mLaserHolder->SetPosition(Vector3(0, 0, 10));
	mLaserComponent = new LaserComponent(mLaserHolder);
	mLaserComponent->SetIgnoreActor(mParent);
}

void TurretHead::OnUpdate(float deltaTime)
{
	mStateTimer += deltaTime;
	switch (mTurretState)
	{
	case TurretState::Idle:
		UpdateIdle(deltaTime);
		break;
	case TurretState::Search:
		UpdateSearch(deltaTime);
		break;
	case TurretState::Priming:
		UpdatePriming(deltaTime);
		break;
	case TurretState::Firing:
		UpdateFiring(deltaTime);
		break;
	case TurretState::Falling:
		UpdateFalling(deltaTime);
		break;
	case TurretState::Dead:
		UpdateDead(deltaTime);
		break;
	}
}

void TurretHead::ChangeState(TurretState newState)
{
	mTurretState = newState;
	mStateTimer = 0.0f;
}

void TurretHead::UpdateIdle(float deltaTime)
{

}

void TurretHead::UpdateSearch(float deltaTime)
{

}

void TurretHead::UpdatePriming(float deltaTime)
{

}

void TurretHead::UpdateFiring(float deltaTime)
{

}

void TurretHead::UpdateFalling(float deltaTime)
{

}

void TurretHead::UpdateDead(float deltaTime)
{
    
}