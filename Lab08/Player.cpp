#include "Player.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "PlayerMove.h"
#include "Random.h"
#include "CameraComponent.h"

Player::Player(Game* game)
: Actor(game)
{
	mGame = game;
	SetScale(0.75f);
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
	mPlayerMove = new PlayerMove(this);
	mCameraComponent = new CameraComponent(this);
	mCameraComponent->SnapToIdeal();
}

void Player::OnUpdate(float deltaTime)
{
}