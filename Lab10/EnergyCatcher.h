#include "Actor.h"
#include <string>

class EnergyCatcher : public Actor
{
public:
	EnergyCatcher(Game* game);
	void SetDoorName(std::string doorName) { mDoorName = doorName; };

private:
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	std::string mDoorName;
};