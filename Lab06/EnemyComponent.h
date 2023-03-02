#pragma once
#include "Component.h"
#include <functional>

class EnemyComponent : public Component
{
public:
	EnemyComponent(class Actor* owner);
	~EnemyComponent();
	void Update(float deltaTime) override;
	class CollisionComponent* GetCollisionComponent() { return mCollisionComponent; };
	void SetHitPoints(int points) { mHitPoints = points; };
	int GetHitPoints() { return mHitPoints; };
	void SetOnDeath(std::function<void()> death) { OnDeath = death; };
	void SetOnDamage(std::function<void()> damage) { OnDamage = damage; };
	void TakeDamage();

protected:
	class CollisionComponent* mCollisionComponent;

private:
	int mHitPoints;
	float mTakeDamageTime = 0.0f;
	std::function<void()> OnDamage;
	std::function<void()> OnDeath;
};
