#include "Actor.h"

class EnergyCube : public Actor
{
public:
	EnergyCube(class Game* game);

private:
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
};