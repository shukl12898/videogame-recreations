#pragma once
#include "VehicleMove.h"
#include <vector>

class EnemyMove : public VehicleMove
{
public:
	EnemyMove(class Actor* owner);
	void Update(float deltaTime) override;

private:
	std::vector<Vector3> mRoute;
	int mNextTarget = 0;
};