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
	mCollisionComponent->SetSize(32, 32);
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(mGame->GetTexture("Assets/Bush.png"));
	mEnemyComponent = new EnemyComponent(this);
	mEnemyComponent->SetHitPoints(1);
	mEnemyComponent->SetOnDeath([this]() {
		float x = this->GetPosition().x;
		float y = this->GetPosition().y;
		this->mGame->GetPathFinder()->SetIsBlocked(y / 32, x / 32, false);
		new Effect(mGame, GetPosition(), "BushDeath", "BushDie.wav");
	});
}
