#include "VehicleMove.h"
#include "Component.h"
#include "Actor.h"
#include "Math.h"
#include "PlayerMove.h"
#include "Game.h"
#include "HeightMap.h"
#include <fstream>
#include "CSVHelper.h"
#include <vector>

VehicleMove::VehicleMove(Actor* owner)
: Component(owner, 50)
{
	mOwner = owner;
	mHeightMap = mOwner->GetGame()->GetHeightMap();

	std::ifstream levelFile;
	levelFile.open("Assets/HeightMap/Checkpoints.csv");

	std::string line;

	std::getline(levelFile, line);

	int rowNum = 0;

	while (std::getline(levelFile, line))
	{
		std::vector<std::string> rowString;
		std::vector<int> rowActual;

		if (!line.empty())
		{
			rowString = (CSVHelper::Split(line));

			int minX = stoi(rowString[1]);
			int maxX = stoi(rowString[2]);
			int minY = stoi(rowString[3]);
			int maxY = stoi(rowString[4]);
			Vector2 minVal(minX, minY);
			Vector2 maxVal(maxX, maxY);

			mCheckpoints.push_back(std::vector<Vector2>());
			mCheckpoints[rowNum].push_back(minVal);
			mCheckpoints[rowNum].push_back(maxVal);
			rowNum++;
		}
	}
}

void VehicleMove::Update(float deltaTime)
{
	if (mAcceleratorPressed)
	{
		//update acceleration time
		mAccelerationTime -= deltaTime;

		//calculate acceleration magnitude based on time
		float time = Math::Clamp<float>(static_cast<float>(mAccelerationTime / RAMP_TIME), 0, 1);
		mAccelerationMag = Math::Lerp(MIN_ACCELERATION, MAX_ACCELERATION, time);

		//euler integrate velocity
		mVelocity += mOwner->GetForward() * mAccelerationMag * deltaTime;
	}
	else
	{
		//reset acceleration time
		mAccelerationTime = RAMP_TIME;
	}

	//euler integrate position
	mOwner->SetPosition(mOwner->GetPosition() + (mVelocity * deltaTime));
	Vector2 position(mOwner->GetPosition().x, mOwner->GetPosition().y);
	if (mOwner->GetGame()->GetHeightMap()->IsOnTrack(position))
	{
		float zPos = Math::Lerp(mOwner->GetPosition().z,
								mOwner->GetGame()->GetHeightMap()->GetHeight(position), 0.1f);
		mOwner->SetPosition(Vector3(position.x, position.y, zPos));
	}

	//apply linear drag to velocity (depends on pedal press)
	if (mAcceleratorPressed)
	{
		mVelocity *= LINEAR_DRAG_ACC;
	}
	else
	{
		mVelocity *= LINEAR_DRAG_NOACC;
	}

	//if vehicle turning, euler integrate angular velocity
	if (mTurnDirection != Turn::None)
	{
		if (mTurnDirection == Turn::Left)
		{
			mAngularVelocity -= ANGULAR_ACCELERATION * deltaTime;
		}
		else if (mTurnDirection == Turn::Right)
		{
			mAngularVelocity += ANGULAR_ACCELERATION * deltaTime;
		}

		//euler integrate angle
		mOwner->SetRotation(mOwner->GetRotation() + mAngularVelocity * deltaTime);

		//apply angular drag
		mAngularVelocity *= ANGULAR_DRAG;
	}

	//figure out if you made it to the "next" checkpoint (increment last checkpoint index)
	int index = mLastCheck + 1;
	if (index >= mCheckpoints.size())
	{
		index = 0;
	}

	Vector2 minPos = mCheckpoints[index][0];
	Vector2 maxPos = mCheckpoints[index][1];
	Vector2 currCoordinates(mOwner->GetPosition().x, mOwner->GetPosition().y);
	Vector2 currCoordinatesCell = mHeightMap->WorldToCell(currCoordinates);
	Vector2 currPos(static_cast<int>(currCoordinatesCell.x),
					static_cast<int>(currCoordinatesCell.y));

	if (((currPos.x >= minPos.x) && (currPos.x <= maxPos.x)) &&
		((currPos.y >= minPos.y) && (currPos.y <= maxPos.y)))
	{
		mLastCheck++;
		if (mLastCheck == mCheckpoints.size())
		{
			mLastCheck = 0;
		}
	}

	//if lap completed (hit checkpoint 0), incremement lap count
	if (mLastCheck == 0 && mStart)
	{
		mStart = false;
		mCurrLap++;
		mLastCheck = 0;
		OnLapChange(mCurrLap);
	}

	if (mLastCheck == 1)
	{
		mStart = true;
	}
}

int VehicleMove::GetDistCheck()
{
	if (mLastCheck > 0 && mLastCheck < mCheckpoints.size())
	{
		Vector2 xyPos(mOwner->GetPosition().x, mOwner->GetPosition().y);
		Vector2 cellPos = mOwner->GetGame()->GetHeightMap()->WorldToCell(xyPos);

		Vector2 checkPos = mCheckpoints[mLastCheck][0];
		return cellPos.Distance(cellPos, checkPos);
	}

	return 0;
}