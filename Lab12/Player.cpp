#include "Player.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "PlayerMove.h"
#include "Random.h"
#include "PlayerMesh.h"
#include "CameraComponent.h"
#include "HealthComponent.h"
#include "HUD.h"

Player::Player(Game* game)
: Actor(game)
{
	mGame = game;
	mPlayerMove = new PlayerMove(this);
	mCameraComponent = new CameraComponent(this);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(50, 100, 50);
	mHealthComponent = new HealthComponent(this);
	mHealthComponent->SetOnDeath([this] {
		mGame->ReloadLevel();
	});
	mHUD = new HUD(this);
}

void Player::GiveGun()
{
	mHasGun = true;
	new PlayerMesh(mGame);
}

void Player::OnUpdate(float deltaTime)
{
}