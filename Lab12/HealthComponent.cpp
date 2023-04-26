#include "Component.h"
#include "HealthComponent.h"
#include "Actor.h"

HealthComponent::HealthComponent(Actor* owner, float initialHealth)
: Component(owner)
{
	mHealth = initialHealth;
}

void HealthComponent::TakeDamage(float damage, const Vector3& location)
{
	if (!IsDead())
	{
		mHealth -= damage;
		if (mOnDamage)
		{
			mOnDamage(location);
		}

		if (IsDead())
		{
			if (mOnDeath)
			{
				mOnDeath();
			}
		}
	}
}
