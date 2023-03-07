#include "Soldier.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "PathNode.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "SoldierAI.h"
#include "EnemyComponent.h"
#include "Effect.h"

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
	mSoldierAI = new SoldierAI(this);
	mSoldierAI->Setup(start, end);
	mEnemyComponent = new EnemyComponent(this);
	mEnemyComponent->SetHitPoints(2);
	mEnemyComponent->SetOnDamage([this]() {
		mSoldierAI->Stunned();
		new Effect(mGame, GetPosition(), "Hit", "EnemyHit.wav");
	});
	mEnemyComponent->SetOnDeath([this]() {
		new Effect(mGame, GetPosition(), "Death", "EnemyDie.wav");
	});
}
