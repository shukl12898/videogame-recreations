#include "Actor.h"

class TurretBase : public Actor
{
public:
	TurretBase(Game* game);
	~TurretBase();

private:
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	class TurretHead* mHead;
};