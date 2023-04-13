#include "Actor.h"
#include <string>

class Pellet : public Actor
{
public:
	Pellet(Game* game);
	void OnUpdate(float deltaTime) override;
	void Teleport(class Portal* entry, class Portal* exit);
	void SetForward(Vector3 forward) { mForward = forward; };
	const float SPEED = 500;

private:
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	Vector3 mForward;
	float mLifetime = 0.0f;
	bool mTeleport = true;
	float mTeleportTime = 0.0f;
	bool mGreen = false;
};