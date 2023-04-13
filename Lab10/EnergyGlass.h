#include "Actor.h"

class EnergyGlass : public Actor
{
public:
	EnergyGlass(class Game* game);

private:
	class CollisionComponent* mCollisionComponent;
	class MeshComponent* mMeshComponent;
};