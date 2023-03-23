#include "EnemyMove.h"
#include "VehicleMove.h"
#include <fstream>
#include "CSVHelper.h"
#include "Actor.h"
#include "Game.h"
#include <vector>
#include "HeightMap.h"

EnemyMove::EnemyMove(Actor* owner)
: VehicleMove(owner)
{
	std::ifstream levelFile;
	levelFile.open("Assets/HeightMap/Path.csv");

	std::string line;

	HeightMap* heightMap = mOwner->GetGame()->GetHeightMap();

	std::getline(levelFile, line);

	int rowNum = 0;

	while (std::getline(levelFile, line))
	{
		std::vector<std::string> rowString;
		std::vector<int> rowActual;

		if (!line.empty())
		{
			rowString = (CSVHelper::Split(line));

			int row = stoi(rowString[1]);
			int col = stoi(rowString[2]);
			Vector3 val = heightMap->CellToWorld(row, col);

			mRoute.push_back(val);
			rowNum++;
		}
	}

	mOwner->SetPosition(mRoute[mNextTarget]);
}

void EnemyMove::Update(float deltaTime)
{
	//figure out if close enough to next target
	Vector3 enemyToTarget = mRoute[mNextTarget] - mOwner->GetPosition();
	enemyToTarget = enemyToTarget.Normalize(enemyToTarget);
	Vector3 forward = mOwner->GetForward();

	//if close enough, advance next target point index (wrap around to 0)
	if (std::abs(mRoute[mNextTarget].Distance(mOwner->GetPosition(), mRoute[mNextTarget])) <= 150)
	{
		mNextTarget++;
		if (mNextTarget >= mRoute.size())
		{
			mNextTarget = 0;
		}
	}

	//figure out if next is reasonably in front, set pedal to true (dot product)
	if (acos(enemyToTarget.Dot(enemyToTarget, forward)) <= (Math::Pi / 10))
	{
		SetPedal(true);
	}

	//figure out if you need to turn, set turn state based on this (dot product, cross product)
	if (std::abs(enemyToTarget.Dot(enemyToTarget, forward)) <= 0.97f)
	{
		Vector3 cross = forward.Cross(forward, enemyToTarget);
		if (cross.z > 0)
		{
			SetTurn(Turn::Right);
		}
		else
		{
			SetTurn(Turn::Left);
		}
	}
	else
	{
		SetTurn(Turn::None);
	}

	VehicleMove::Update(deltaTime);
}