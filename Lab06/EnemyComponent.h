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
	const int GetHitPoints() const { return mHitPoints; };
	void SetOnDeath(std::function<void()> death) { mOnDeath = death; };
	void SetOnDamage(std::function<void()> damage) { mOnDamage = damage; };
	void TakeDamage();

protected:
	class CollisionComponent* mCollisionComponent;

private:
	int mHitPoints = 0;
	float mTakeDamageTime = 0.0f;
	std::function<void()> mOnDamage;
	std::function<void()> mOnDeath;
};
