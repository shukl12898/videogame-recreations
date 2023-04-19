#include "Actor.h"
#include <string>

class Door : public Actor
{
public:
	Door(Game* game, std::string name);
	~Door();
	void OpenDoor() { mOpen = true; };
	bool GetOpen() const { return mOpen; };

private:
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	std::string mName;
	bool mOpen = false;
};