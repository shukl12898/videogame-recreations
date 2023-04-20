#include "Actor.h"

enum class TurretState
{
	Idle,
	Search,
	Priming,
	Firing,
	Falling,
	Dead
};

class TurretHead : public Actor
{
public:
	TurretHead(Game* game, Actor* parent);
	void UpdateIdle(float deltaTime);
	void UpdateSearch(float deltaTime);
	void UpdatePriming(float deltaTime);
	void UpdateFiring(float deltaTime);
	void UpdateFalling(float deltaTime);
	void UpdateDead(float deltaTime);
	void ChangeState(TurretState newState);
	void OnUpdate(float deltaTime) override;

private:
	class MeshComponent* mMeshComponent;
	Actor* mLaserHolder;
	class LaserComponent* mLaserComponent;
	TurretState mTurretState = TurretState::Idle;
	float mStateTimer = 0.0f;
};