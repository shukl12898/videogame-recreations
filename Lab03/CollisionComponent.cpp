#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
: Component(owner)
, mWidth(0.0f)
, mHeight(0.0f)
{
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::Intersect(const CollisionComponent* other) const
{
	bool noIntersection = GetMax().x < other->GetMin().x || other->GetMax().x < GetMin().x ||
						  GetMax().y < other->GetMin().y || other->GetMax().y < GetMin().y;
	return !noIntersection;
}

Vector2 CollisionComponent::GetMin() const
{
	float minX = mOwner->GetPosition().x - (mWidth * mOwner->GetScale()) / 2.0f;
	float minY = mOwner->GetPosition().y - (mHeight * mOwner->GetScale()) / 2.0f;
	return Vector2(minX, minY);
}

Vector2 CollisionComponent::GetMax() const
{
	float maxX = mOwner->GetPosition().x + (mWidth * mOwner->GetScale()) / 2.0f;
	float maxY = mOwner->GetPosition().y + (mHeight * mOwner->GetScale()) / 2.0f;
	return Vector2(maxX, maxY);
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other, Vector2& offset) const
{

	CollSide result = CollSide::None;

	if (!Intersect(other))
	{
		return result;
	}

	Vector2 otherMin = other->GetMin();
	Vector2 otherMax = other->GetMax();
	Vector2 thisMin = GetMin();
	Vector2 thisMax = GetMax();

	float topDist = otherMin.y - thisMax.y;
	float bottomDist = otherMax.y - thisMin.y;
	float rightDist = otherMin.x - thisMin.x;
	float leftDist = otherMax.x - thisMax.x;

	float minDist = std::min(topDist, std::min(bottomDist, std::min(rightDist, leftDist)));

	offset = Vector2::Zero;

	// if (minDist == topDist)
	// {
	// 	offset.y = otherMin.y;
	// 	result = CollSide::Top;
	// }

	// if (minDist == bottomDist)
	// {
	// 	offset.y = otherMax.y;
	// 	result = CollSide::Bottom;
	// }

	// if (minDist == leftDist)
	// {
	// 	offset.x = otherMin.x;
	// 	result = CollSide::Left;
	// }

	// if (minDist == rightDist)
	// {
	// 	offset.x = otherMax.x;
	// 	result = CollSide::Right;
	// }

	return CollSide::Top;
}
