#include "Actor.h"
#include <string>

class Door : public Actor
{
public:
	Door(Game* game, std::string name);
	~Door();
	void OnUpdate(float deltaTime) override;
	void OpenDoor() { mOpen = true; };
	bool GetOpen() { return mOpen; };

private:
	class MeshComponent* mMeshComponent;
	class CollisionComponent* mCollisionComponent;
	std::string mName;
	bool mOpen = false;
	Actor* mLeft = nullptr;
	Actor* mRight = nullptr;
	float mOpenTime = 0.0f;
};