#include "Actor.h"
#include <string>

class EnergyLauncher : public Actor
{
public:
	EnergyLauncher(Game* game);
	~EnergyLauncher();
	void SetCooldown(float cooldown) { mCooldown = cooldown; };
	void SetDoorName(std::string doorName) { mDoorName = doorName; };
	void OnUpdate(float deltaTime) override;

private:
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	std::string mDoorName;
	float mCooldown;
	float pelletTimer = 0.0f;
	bool mLaunch = true;
};