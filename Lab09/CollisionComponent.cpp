#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
: Component(owner)
, mWidth(0.0f)
, mHeight(0.0f)
, mDepth(0.0f)
{
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::Intersect(const CollisionComponent* other) const
{
	Vector3 thisMin = GetMin();
	Vector3 thisMax = GetMax();
	Vector3 otherMin = other->GetMin();
	Vector3 otherMax = other->GetMax();

	bool noIntersection = thisMax.x < otherMin.x || thisMax.y < otherMin.y ||
						  thisMax.z < otherMin.z || otherMax.x < thisMin.x ||
						  otherMax.y < thisMin.y || otherMax.z < thisMin.z;

	return !noIntersection;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale().x / 2.0f;
	v.y -= mWidth * mOwner->GetScale().y / 2.0f;
	v.z -= mHeight * mOwner->GetScale().z / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale().x / 2.0f;
	v.y += mWidth * mOwner->GetScale().y / 2.0f;
	v.z += mHeight * mOwner->GetScale().z / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other, Vector3& offset) const
{
	offset = Vector3::Zero;
	CollSide result = CollSide::None;

	if (!Intersect(other))
	{
		return result;
	}
	Vector3 otherMin = other->GetMin();
	Vector3 otherMax = other->GetMax();
	Vector3 thisMin = GetMin();
	Vector3 thisMax = GetMax();

	float topDist = std::abs(otherMax.z - thisMin.z);
	float bottomDist = std::abs(otherMin.z - thisMax.z);
	float rightDist = std::abs(otherMax.y - thisMin.y);
	float leftDist = std::abs(otherMin.y - thisMax.y);
	float frontDist = std::abs(otherMax.x - thisMin.x);
	float backDist = std::abs(otherMin.x - thisMax.x);

	float minDist = std::min(
		topDist, std::min(bottomDist,
						  std::min(rightDist, std::min(leftDist, std::min(frontDist, backDist)))));

	if (minDist == topDist)
	{
		offset.z = -topDist;
		result = CollSide::Top;
	}

	if (minDist == bottomDist)
	{
		offset.z = bottomDist;
		result = CollSide::Bottom;
	}

	if (minDist == leftDist)
	{
		offset.y = -leftDist;
		result = CollSide::Left;
	}

	if (minDist == rightDist)
	{
		offset.y = rightDist;
		result = CollSide::Right;
	}

	if (minDist == frontDist)
	{
		offset.x = frontDist;
		result = CollSide::Front;
	}

	if (minDist == backDist)
	{
		offset.x = backDist;
		result = CollSide::Back;
	}

	return result;
}
