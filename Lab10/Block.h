#include "Actor.h"

class Block : public Actor
{
public:
	Block(class Game* game);
	~Block();

private:
	class MeshComponent* mMeshComponent = nullptr;
	class CollisionComponent* mCollisionComponent = nullptr;
};