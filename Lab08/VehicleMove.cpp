#include "VehicleMove.h"
#include "Component.h"
#include "Actor.h"
#include "Math.h"
#include "PlayerMove.h"

VehicleMove::VehicleMove(Actor* owner)
: Component(owner, 50)
{
	mOwner = owner;
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
}