#include "Actor.h"

class TurretHead : public Actor
{
public:
	TurretHead(Game* game, Actor* parent);
	void OnUpdate(float deltaTime) override;

private:
	class MeshComponent* mMeshComponent;
};