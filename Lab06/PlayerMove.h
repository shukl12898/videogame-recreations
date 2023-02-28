#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include <string>

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Player* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState) override;

protected:
	std::string DetermineAnimation();
	Game* mGame = nullptr;
	class Player* mOwner = nullptr;
	bool mSpacePressed = false;
};