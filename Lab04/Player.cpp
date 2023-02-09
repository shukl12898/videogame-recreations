#include "Player.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"

Player::Player(Game* game)
: Actor(game)
{
	mGame = game;
	mSpriteComponent = new SpriteComponent(this, 200);
	mSpriteComponent->SetTexture(game->GetTexture("Assets/Mario/Idle.png"));
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(32, 32);
	mPlayerMove = new PlayerMove(this);
}
