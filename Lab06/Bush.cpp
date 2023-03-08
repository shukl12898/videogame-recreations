#include "Bush.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "EnemyComponent.h"
#include "PathFinder.h"
#include "Effect.h"

Bush::Bush(Game* game)
: Actor(game)
{
	mGame = game;
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(BUSH_SIZE, BUSH_SIZE);
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(mGame->GetTexture("Assets/Bush.png"));
	mEnemyComponent = new EnemyComponent(this);
	mEnemyComponent->SetHitPoints(BUSH_HIT_POINTS);
	mEnemyComponent->SetOnDeath([this]() {
		float x = GetPosition().x;
		float y = GetPosition().y;
		mGame->GetPathFinder()->SetIsBlocked(static_cast<size_t>(y / BUSH_SIZE),
											 static_cast<size_t>(x / BUSH_SIZE), false);
		new Effect(mGame, GetPosition(), "BushDeath", "BushDie.wav");
	});
}
