#include "Soldier.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "PathNode.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "SoldierAI.h"

Soldier::Soldier(Game* game, PathNode* start, PathNode* end)
: Actor(game)
{
	mGame = game;
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(32, 32);
	mASC = new AnimatedSprite(this);
	mASC->LoadAnimations("Assets/Soldier");
	mASC->SetAnimation("WalkDown");
	mASC->SetAnimFPS(5.0f);
	SoldierAI* soldierAI = new SoldierAI(this);
	soldierAI->Setup(start, end);
}
