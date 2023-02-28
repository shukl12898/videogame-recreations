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
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(32, 32);
	mPlayerMove = new PlayerMove(this);
}
