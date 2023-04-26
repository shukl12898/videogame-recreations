#include "Portal.h"
#include "Game.h"
#include "Actor.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"
#include "Player.h"
#include <vector>

Portal::Portal(Game* game)
: Actor(game)
{
	mPortalMesh = new PortalMeshComponent(this);
}

void Portal::OnUpdate(float deltaTime)
{
	if (this == mGame->GetBluePortal())
	{
		GetGame()->GetRenderer()->GetBluePortal().mView = CalcViewMatrix(mGame->GetOrangePortal());
	}
	else
	{
		GetGame()->GetRenderer()->GetOrangePortal().mView = CalcViewMatrix(mGame->GetBluePortal());
	}
}

Matrix4 Portal::CalcViewMatrixBasic(Portal* other)
{
	if (other != nullptr)
	{
		//eye: 5 units BEHIND opposite portal
		Vector3 eye = other->GetPosition() - other->GetQuatForward() * 5;
		//target: 5 units in front of opposite portal
		Vector3 target = other->GetPosition() + other->GetQuatForward() * 5;
		//up: get world transform matrix from other, use get z axis to get correct up vector
		Vector3 up = other->GetWorldTransform().GetZAxis();

		//use Matrix4::CreateLookAt
		return Matrix4::CreateLookAt(eye, target, up);
	}
	else
	{
		return Matrix4::CreateScale(0.0f);
	}
}

Matrix4 Portal::CalcViewMatrix(Portal* other)
{
	if (other == nullptr)
	{
		return Matrix4::CreateScale(0.0f);
	}
	else
	{
		if (!Math::NearZero(GetQuatForward().z))
		{
			return CalcViewMatrixBasic(other);
		}
		else
		{
			//calculate vector from player to "in" portal
			Vector3 playerToIn = GetPosition() - mGame->GetPlayer()->GetPosition();

			//if length of vector is 0, return matrix you saved in member variable in 9 as that was valid view matrix from lsat frame
			if (playerToIn.Length() != 0)
			{
				Vector3 outWorld = GetOut(playerToIn, other);
				//eye: 5 units BEHIND opposite portal
				Vector3 eye = other->GetPosition() - outWorld * 5;
				//target: 5 units in front of opposite portal
				Vector3 target = other->GetPosition();
				//up: get world transform matrix from other, use get z axis to get correct up vector
				Vector3 up = other->GetWorldTransform().GetZAxis();

				mComplexView = Matrix4::CreateLookAt(eye, target, up);

				outWorld.z = 0.0f;
				outWorld.Normalize();

				Vector3 initialFacing = Vector3::UnitX;
				initialFacing.Normalize();
				float dotProduct = initialFacing.Dot(initialFacing, outWorld);
				float theta = Math::Acos(dotProduct);

				Vector3 cross = initialFacing.Cross(initialFacing, outWorld);
				if (cross.z < 0)
				{
					theta = -1 * theta;
				}
				mOutYaw = theta;
			}

			return mComplexView;
		}
	}
}

Vector3 Portal::GetOut(Vector3 playerToIn, Portal* other)
{

	//else, noramlize player to "in" vector
	playerToIn.Normalize();
	//get inverse world transform matrix of "in" portal
	Matrix4 inverseWT = GetWorldTransform();
	inverseWT.Invert();
	//use vector3::transform to transform vector from player to "in" by inverse world transform, 0.0f as third parameter
	Vector3 playerToInObj = Vector3::Transform(playerToIn, inverseWT, 0.0f);
	//create z rotation matrix rotating by math::pi
	Matrix4 zRotation = Matrix4::CreateRotationZ(Math::Pi);
	//transform playerToInObj by z rotation matrix
	Vector3 outObj = Vector3::Transform(playerToInObj, zRotation, 0.0f);
	//transform outObj by "out" portal's world transform
	Vector3 outWorld = Vector3::Transform(outObj, other->GetWorldTransform(), 0.0f);
	//normalize outworld
	outWorld.Normalize();

	return outWorld;
}