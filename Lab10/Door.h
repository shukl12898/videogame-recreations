#include "Actor.h"
#include <string>

class Door : public Actor
{
public:
	Door(Game* game);
	void SetName(std::string name) { mName = name; };

private:
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	std::string mName;
};