#pragma once
#include "Actor.h"
#include <string>

class Effect : public Actor
{
public:
	Effect(class Game* game, Vector2 position, std::string animName, std::string soundName);
	void OnUpdate(float deltaTime) override;

protected:
	class Game* mGame;
	class CollisionComponent* mCollisionComponent;
	float mLifetime;
};