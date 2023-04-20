#include "LaserComponent.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Actor.h"
#include "Game.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Portal.h"
#include <vector>

LaserComponent::LaserComponent(Actor* owner)
: MeshComponent(owner, true)
{
	SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh("Assets/Meshes/Laser.gpmesh"));
}

void LaserComponent::Update(float deltaTime)
{
	//clear out line segment vector
	mLineSegments.clear();
	mLastHitActor = nullptr;

	if (!mDisabled)
	{

		//create first line segment and insert into vector
		LineSegment firstSegment(mOwner->GetWorldPosition(),
								 mOwner->GetWorldPosition() + mOwner->GetWorldForward() * 350);

		CastInfo castInfo;
		bool intersect =
			SegmentCast(mOwner->GetGame()->GetActors(), firstSegment, castInfo, mIgnoreActor);
		if (intersect)
		{
			firstSegment.mEnd = castInfo.mPoint;
		}

		mLineSegments.emplace_back(firstSegment);

		//create an additional segment and insert into vector if needed
		Portal* orangePortal = mOwner->GetGame()->GetOrangePortal();
		Portal* bluePortal = mOwner->GetGame()->GetBluePortal();
		Portal* in = dynamic_cast<Portal*>(castInfo.mActor);

		if (in == nullptr)
		{
			if (intersect)
			{
				mLastHitActor = castInfo.mActor;
			}
			else
			{
				mLastHitActor = nullptr;
			}
		}

		//Intersects with a portal
		if ((in != nullptr) && (orangePortal != nullptr) && (bluePortal != nullptr))
		{
			Portal* out = orangePortal;
			if (in == orangePortal)
			{
				out = bluePortal;
			}

			Vector3 currDirection = firstSegment.mEnd - firstSegment.mStart;
			currDirection.Normalize();
			Vector3 direction = in->GetOut(currDirection, out);
			Vector3 start = out->GetPosition() + 0.5 * direction;
			LineSegment secondSegment(start, start + 350 * direction);

			CastInfo secondCastInfo;
			bool secondIntersect =
				SegmentCast(mOwner->GetGame()->GetActors(), secondSegment, secondCastInfo, out);
			if (secondIntersect)
			{
				secondSegment.mEnd = secondCastInfo.mPoint;
				mLastHitActor = secondCastInfo.mActor;
			}
			else
			{
				mLastHitActor = nullptr;
			}
			mLineSegments.emplace_back(secondSegment);
		}
	}
}

Matrix4 LaserComponent::LaserTransform(LineSegment segment)
{
	Matrix4 scale = Matrix4::CreateScale(segment.Length(), 1.0f, 1.0f);
	Vector3 desiredFacing = segment.mEnd - segment.mStart;
	desiredFacing.Normalize();
	Vector3 originalFacing = Vector3::UnitX;
	Quaternion segmentFacing;

	float dotProduct = originalFacing.Dot(originalFacing, desiredFacing);

	if (dotProduct == 1)
	{
		segmentFacing = Quaternion::Identity;
	}
	else if (dotProduct == -1)
	{
		segmentFacing = Quaternion(Vector3::UnitZ, Math::Pi);
	}
	else
	{
		//normalized axis of rotation
		Vector3 axisRotation = desiredFacing.Cross(originalFacing, desiredFacing);
		Vector3 normalizedAxisRotation = axisRotation.Normalize(axisRotation);

		//angle of rotation
		float theta = Math::Acos(dotProduct);

		segmentFacing = Quaternion(normalizedAxisRotation, theta);
	}
	Matrix4 rotation = Matrix4::CreateFromQuaternion(segmentFacing);
	Matrix4 translation = Matrix4::CreateTranslation(segment.PointOnSegment(0.5));
	Matrix4 transform = scale * rotation * translation;

	return transform;
}

void LaserComponent::Draw(Shader* shader)
{
	for (int i = 0; i < mLineSegments.size(); i++)
	{
		if (mMesh)
		{
			// Set the world transform
			shader->SetMatrixUniform("uWorldTransform", LaserTransform(mLineSegments[i]));
			// Set the active texture
			Texture* t = mMesh->GetTexture(mTextureIndex);
			if (t)
			{
				t->SetActive();
			}
			// Set the mesh's vertex array as active
			VertexArray* va = mMesh->GetVertexArray();
			va->SetActive();
			// Draw
			glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		}
	}
}