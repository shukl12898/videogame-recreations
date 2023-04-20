#include "Actor.h"

class TurretBase : public Actor
{
public:
	TurretBase(Game* game);
	void Die();
	~TurretBase();

private:

	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	class HealthComponent* mHealthComponent;
	class TurretHead* mHead;
};