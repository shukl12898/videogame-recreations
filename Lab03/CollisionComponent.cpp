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
	offset = Vector2::Zero;
	// TODO: Implement
	return CollSide::None;
}
