#include "Player.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"

Player::Player(Game* game)
: Actor(game)
{
	mGame = game;
	mASC = new AnimatedSprite(this, 200);
	mASC->LoadAnimations("Assets/Link");
	mASC->SetAnimation("StandDown");
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(20, 20);
	mPlayerMove = new PlayerMove(this);
}
