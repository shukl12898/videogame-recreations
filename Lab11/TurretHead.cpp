#include "TurretHead.h"
#include "Mesh.h"
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "LaserComponent.h"
#include "TurretBase.h"
#include "HealthComponent.h"
#include "Random.h"
#include "Portal.h"

TurretHead::TurretHead(Game* game, Actor* parent)
: Actor(game, parent)
{
	mGame = game;
	SetScale(0.75f);
	mPosition = Vector3(0.0f, 0.0f, 18.75f);
	mMeshComponent = new MeshComponent(this);
	Mesh* mesh = mGame->GetRenderer()->GetMesh("Assets/Meshes/TurretHead.gpmesh");
	mMeshComponent->SetMesh(mesh);
	mLaserHolder = new Actor(game, this);
	mLaserHolder->SetPosition(Vector3(0, 0, 10));
	mLaserComponent = new LaserComponent(mLaserHolder);
	mLaserComponent->SetIgnoreActor(mParent);
}

bool TurretHead::TargetAcquistion()
{
	Actor* lastHit = mLaserComponent->GetLastHitActor();

	if (lastHit != nullptr)
	{
		HealthComponent* hc = lastHit->GetComponent<HealthComponent>();
		if (hc != nullptr)
		{
			if (!hc->IsDead())
			{
				mTarget = lastHit;
				return true;
			}
		}
	}

	return false;
}

void TurretHead::OnUpdate(float deltaTime)
{
	mStateTimer += deltaTime;
	switch (mTurretState)
	{
	case TurretState::Idle:
		UpdateIdle(deltaTime);
		break;
	case TurretState::Search:
		UpdateSearch(deltaTime);
		break;
	case TurretState::Priming:
		UpdatePriming(deltaTime);
		break;
	case TurretState::Firing:
		UpdateFiring(deltaTime);
		break;
	case TurretState::Falling:
		UpdateFalling(deltaTime);
		break;
	case TurretState::Dead:
		UpdateDead(deltaTime);
		break;
	}
}

void TurretHead::ChangeState(TurretState newState)
{
	mTurretState = newState;
	mStateTimer = 0.0f;
}

void TurretHead::UpdateIdle(float deltaTime)
{
	if (PortalTeleport(deltaTime))
	{
		ChangeState(TurretState::Falling);
		return;
	}
	if (TargetAcquistion())
	{
		ChangeState(TurretState::Priming);
	}
}

void TurretHead::UpdateSearch(float deltaTime)
{
	if (PortalTeleport(deltaTime))
	{
		ChangeState(TurretState::Falling);

		return;
	}
	if (TargetAcquistion())
	{
		ChangeState(TurretState::Priming);
		return;
	}

	mSearchTimer += deltaTime;

	if (!mFirstRound)
	{
		Vector3 center = GetPosition() + FWD_DIST * Vector3::UnitX;
		float theta = Random::GetFloatRange(-Math::Pi, Math::Pi);
		Vector3 offset(0.0f, SIDE_DIST * cos(theta), UP_DIST * sin(theta));
		mRandomPoint = center + offset;
		mFirstRound = true;
		Vector3 unit = mRandomPoint - GetPosition();
		unit.Normalize();

		Vector3 originalFacing = Vector3::UnitX;
		Quaternion segmentFacing;

		float dotProduct = originalFacing.Dot(originalFacing, unit);

		//normalized axis of rotation
		Vector3 axisRotation = unit.Cross(originalFacing, unit);
		Vector3 normalizedAxisRotation = axisRotation.Normalize(axisRotation);

		//angle of rotation
		float angle = Math::Acos(dotProduct);

		mAimQuat = Quaternion(normalizedAxisRotation, angle);
		mFirstRound = true;
	}

	float time = Math::Clamp<float>(static_cast<float>(mStateTimer / 0.5, 0, 1));
	if (mFirstRound && !mSecondRound)
	{
		SetQuat(Quaternion::Slerp(Quaternion::Identity, mAimQuat, time));
		if (mStateTimer >= 0.5f)
		{
			mStateTimer = 0.0f;
			mSecondRound = true;
		}
	}
	if (mSecondRound)
	{
		SetQuat(Quaternion::Slerp(mAimQuat, Quaternion::Identity, time));
		if (mStateTimer >= 0.5f)
		{
			mStateTimer = 0.0f;
			mSecondRound = false;
			mFirstRound = false;
		}
	}
	if (mSearchTimer > 5.0f)
	{
		mSearchTimer = 0.0f;
		ChangeState(TurretState::Idle);
	}
}

void TurretHead::UpdatePriming(float deltaTime)
{
	if (PortalTeleport(deltaTime))
	{
		ChangeState(TurretState::Falling);
		return;
	}
	if (mTarget != mLaserComponent->GetLastHitActor())
	{
		ChangeState(TurretState::Search);
	}
	else
	{
		if (mStateTimer >= 1.5)
		{
			ChangeState(TurretState::Firing);
			mFireTimer = 0.05f;
		}
	}
}

void TurretHead::UpdateFiring(float deltaTime)
{
	if (PortalTeleport(deltaTime))
	{
		ChangeState(TurretState::Falling);
		return;
	}
	if (mTarget != mLaserComponent->GetLastHitActor())
	{
		ChangeState(TurretState::Search);
		return;
	}

	if (!mTarget->GetComponent<HealthComponent>()->IsDead())
	{
		mFireTimer -= deltaTime;
		if (mFireTimer <= 0.0f)
		{
			mTarget->GetComponent<HealthComponent>()->TakeDamage(2.5, GetWorldPosition());
			mFireTimer = 0.05f;
		}
	}
}

void TurretHead::UpdateFalling(float deltaTime)
{
	mParent->SetPosition(mParent->GetPosition() + mFallVelocity * deltaTime);

	if (!PortalTeleport(deltaTime))
	{
		mFallVelocity += deltaTime * Vector3(0, 0, -980);
		std::vector<Actor*> colliders = mGame->GetColliders();
		for (int i = 0; i < colliders.size(); i++)
		{
			if (colliders[i] != mParent)
			{
				Vector3 offset;
				CollSide result = mParent->GetComponent<CollisionComponent>()->GetMinOverlap(
					colliders[i]->GetComponent<CollisionComponent>(), offset);

				if (result != CollSide::None)
				{
					mParent->SetPosition(mParent->GetPosition() + offset);
				}

				if (result == CollSide::Top && mFallVelocity.z < 0.0f)
				{
					Vector3 currPos = mParent->GetPosition();
					Vector3 update(currPos.x, currPos.y, currPos.z - 15);
					mParent->SetPosition(update);
					Die();

					TurretBase* base = dynamic_cast<TurretBase*>(colliders[i]);
					if (base != nullptr)
					{
						float height =
							(mParent->GetComponent<CollisionComponent>()->GetHeight()) / 2;
						currPos = mParent->GetPosition();
						Vector3 position(currPos.x, currPos.y, currPos.z - height);
						mParent->SetPosition(position);
						base->Die();
					}
				}
			}
		}
	}

	if (mFallVelocity.Length() > 800.0f)
	{
		mFallVelocity.Normalize();
		mFallVelocity *= 800.0f;
	}
}

void TurretHead::UpdateDead(float deltaTime)
{
}

void TurretHead::Die()
{
	ChangeState(TurretState::Dead);
	Quaternion quat = Quaternion(Vector3::UnitX, Math::PiOver2);
	mParent->SetQuat(quat);
	mLaserComponent->Disable();
}

bool TurretHead::PortalTeleport(float deltaTime)
{
	//only teleport if there's both a blue and orange portal
	mCountdown -= deltaTime;
	Portal* blue = mGame->GetBluePortal();
	Portal* orange = mGame->GetOrangePortal();

	if (blue != nullptr && orange != nullptr)
	{
		//can't teleport if it's been less than 0.25 sedconds since a previous portal teleport
		if (mCountdown <= 0.0f)
		{
			//only initiate teleport if you intersect with one of the portals, intersected is entry, opposite is exit
			if (blue->GetComponent<CollisionComponent>()->Intersect(
					mParent->GetComponent<CollisionComponent>()))
			{
				mParent->SetPosition(orange->GetPosition());
				mFallVelocity += orange->GetQuatForward() * 250;
				mCountdown = 0.25f;
				//make sure update portal teleport returns true
				return true;
			}
			else if (orange->GetComponent<CollisionComponent>()->Intersect(
						 mParent->GetComponent<CollisionComponent>()))
			{
				mParent->SetPosition(blue->GetPosition());
				mFallVelocity += blue->GetQuatForward() * 250;
				mCountdown = 0.25f;
				//make sure update portal teleport returns true
				return true;
			}
		}
	}

	return false;
}