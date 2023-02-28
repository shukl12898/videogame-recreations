#include "PlayerMove.h"
#include "Player.h"
#include "Actor.h"
#include "Game.h"
#include "Block.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Math.h"
#include <string>

PlayerMove::PlayerMove(Player* owner)
: MoveComponent(owner)
{

	mGame = owner->GetGame();
	mOwner = owner;
}

void PlayerMove::Update(float deltaTime)
{
}

std::string PlayerMove::DetermineAnimation()
{
	return "";
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
}