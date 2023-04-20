#include "Component.h"
#include <functional>

class HealthComponent : public Component
{
public:
	HealthComponent(class Actor* owner, float initialHealth = 100.0f);
	void SetOnDeath(std::function<void()> death) { mOnDeath = death; };
	void SetOnDamage(std::function<void(const Vector3&)> damage) { mOnDamage = damage; };
	float GetHealth() { return mHealth; }
	bool IsDead() { return mHealth <= 0.0f; }
	void TakeDamage(float damage, const Vector3& location);

private:
	float mHealth;
	std::function<void(const Vector3&)> mOnDamage;
	std::function<void()> mOnDeath;
};