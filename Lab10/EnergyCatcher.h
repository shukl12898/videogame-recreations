#include "Actor.h"
#include <string>

class EnergyCatcher : public Actor
{
public:
	EnergyCatcher(Game* game, std::string doorName);
	~EnergyCatcher();
	void Activate();

private:
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	std::string mDoorName;
	bool mActivated = false;
};