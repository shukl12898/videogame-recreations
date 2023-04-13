#include "Actor.h"

class EnergyGlass : public Actor
{
public:
	EnergyGlass(class Game* game);
	~EnergyGlass();

private:
	class CollisionComponent* mCollisionComponent;
	class MeshComponent* mMeshComponent;
};