#include "Player.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "PlayerMove.h"

Player::Player(Game* game)
: Actor(game)
{
	mGame = game;
	SetScale(2.0f);
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Arwing.gpmesh"));
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(40.0f, 25.0f, 15.0f);
	mPlayerMove = new PlayerMove(this);
}
