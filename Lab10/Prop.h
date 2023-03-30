#include "Actor.h"
#include <string>

class Prop : public Actor
{
public:
	Prop(std::string mesh, bool usesAlpha, bool hasCollisions, class Game* game);
	~Prop();

private:
	class MeshComponent* mMeshComponent = nullptr;
	class CollisionComponent* mCollisionComponent = nullptr;
};