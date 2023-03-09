#include "Player.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "PlayerMove.h"
#include "HUD.h"
#include "Random.h"

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
	mHUD = new HUD(this);
}

void Player::OnUpdate(float deltaTime)
{
	mCooldown -= deltaTime;

	if (mCooldown <= 0 && mPlayerMove->GetShieldLevel() != 3)
	{
		mHUD->DoABarrelRoll();
		mCooldown = Random::GetFloatRange(15, 25);
	}
}