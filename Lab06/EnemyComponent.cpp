#include "Component.h"
#include "Game.h"
#include "EnemyComponent.h"
#include "CollisionComponent.h"
#include "Actor.h"

EnemyComponent::EnemyComponent(Actor* owner)
: Component(owner)
{
	mOwner->GetGame()->GetEnemyComponents().push_back(this);
	mCollisionComponent = mOwner->GetComponent<CollisionComponent>();
}

EnemyComponent::~EnemyComponent()
{
	mOwner->GetGame()->GetEnemyComponents().erase(
		std::find(mOwner->GetGame()->GetEnemyComponents().begin(),
				  mOwner->GetGame()->GetEnemyComponents().end(), this));
}

void EnemyComponent::Update(float deltaTime)
{
	mTakeDamageTime -= deltaTime;
}

void EnemyComponent::TakeDamage()
{
	if (mTakeDamageTime <= 0)
	{
		mHitPoints -= 1;
		mTakeDamageTime = DAMAGE_TIME_MAX;
		if (mHitPoints == 0)
		{
			if (mOnDeath)
			{
				mOnDeath();
			}
			mOwner->SetState(ActorState::Destroy);
		}
		else
		{
			if (mOnDamage)
			{
				mOnDamage();
			}
		}
	}
}