#include "Enemy.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "EnemyMove.h"

Enemy::Enemy(Game* game)
: Actor(game)
{
	mGame = game;
	SetScale(0.75f);
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
	mMeshComp->SetTextureIndex(6);
	mEnemyMove = new EnemyMove(this);
}