#include "PlayerMove.h"
#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Random.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "Player.h"
#include "Crosshair.h"
#include "SegmentCast.h"
#include "Portal.h"
#include "MeshComponent.h"
#include "Block.h"
#include "HealthComponent.h"
#include "AudioSystem.h"

PlayerMove::PlayerMove(Actor* owner)
: MoveComponent(owner)
{
	mOwner = owner;
	mGame = mOwner->GetGame();
	ChangeState(OnGround);
	mGravity = Vector3(0.0f, 0.0f, -980.0f);
	mJumpForce = Vector3(0.0f, 0.0f, 35000.0f);
	mCrosshair = new Crosshair(mOwner);
	mAudio = mGame->GetAudio();
	mFootStepSound = mAudio->PlaySound("FootstepLoop.ogg", true);
	mAudio->PauseSound(mFootStepSound);
}

PlayerMove::~PlayerMove()
{
	mAudio->StopSound(mFootStepSound);
}

void PlayerMove::Update(float deltaTime)
{

	if (mOwner->GetComponent<HealthComponent>()->IsDead())
	{
		if (mGame->GetAudio()->GetSoundState(mGame->GetPlayer()->GetDeadSound()) ==
			SoundState::Stopped)
		{
			mGame->ReloadLevel();
		}
		else
		{
			return;
		}
	}

	if (mOwner->GetPosition().z <= -750)
	{
		mOwner->GetComponent<HealthComponent>()->TakeDamage(Math::Infinity, mOwner->GetPosition());
	}

	switch (mCurrentState)
	{
	case OnGround:
		UpdateOnGround(deltaTime);
		break;
	case Jump:
		UpdateJump(deltaTime);
		break;
	case Falling:
		UpdateFalling(deltaTime);
		break;
	}

	if (mCurrentState == MoveState::OnGround && mVelocity.Length() > 50)
	{
		mAudio->ResumeSound(mFootStepSound);
	}
	else
	{
		mAudio->PauseSound(mFootStepSound);
	}
}

void PlayerMove::Teleport(Portal* entry, Portal* exit)
{

	//player should change to position of exit portal
	mOwner->SetPosition(exit->GetPosition());
	//change state to falling
	ChangeState(Falling);
	//use quat get forward for direction of portals
	Vector3 entryDirection = entry->GetQuatForward();
	Vector3 exitDirection = exit->GetQuatForward();
	//calculate magnitude m of current velocity in direction of entry portal
	float magnitudeEntry = (mVelocity.Dot(entryDirection, mVelocity));
	float magnitudeExit = -1.5f * magnitudeEntry;
	//magnitude of exit should either be 1.5 m or 350.0f (whichever is higher)
	if (magnitudeExit < 350.0f)
	{
		magnitudeExit = 350.0f;
	}
	//direction of exit velocity is in direction of exit portal
	mVelocity = magnitudeExit * exitDirection;
	//set bool that says falling due to portal
	mTeleporting = true;
	mCountdown = 2.0f;

	//rotate player's yaw (if along +/-  x or +/- y)
	if (Math::NearZero(exitDirection.z))
	{
		mOwner->SetRotation(entry->GetOutYaw());
	}
}

bool PlayerMove::UpdatePortalTeleport(float deltaTime)
{
	//only teleport if there's both a blue and orange portal
	mCountdown -= deltaTime;
	Portal* blue = mGame->GetBluePortal();
	Portal* orange = mGame->GetOrangePortal();

	if (blue != nullptr && orange != nullptr)
	{
		//can't teleport if it's been less than 0.2 sedconds since a previous portal teleport
		if (mCountdown <= 0.0f)
		{
			//only initiate teleport if you intersect with one of the portals, intersected is entry, opposite is exit
			if (blue->GetComponent<CollisionComponent>()->Intersect(
					mOwner->GetComponent<CollisionComponent>()))
			{
				Teleport(blue, orange);
				mAudio->PlaySound("PortalTeleport.ogg");
				//make sure update portal teleport returns true
				return true;
			}
			else if (orange->GetComponent<CollisionComponent>()->Intersect(
						 mOwner->GetComponent<CollisionComponent>()))
			{
				Teleport(orange, blue);
				mAudio->PlaySound("PortalTeleport.ogg");
				//make sure update portal teleport returns true
				return true;
			}
		}
	}

	return false;
}

void PlayerMove::CreatePortal(bool isBlue)
{
	Vector3 start = mGame->GetRenderer()->Unproject(Vector3(0.0f, 0.0f, 0.0f));
	Vector3 end = mGame->GetRenderer()->Unproject(Vector3(0.0f, 0.0f, 0.9f));
	Vector3 segmentDirection = (end - start).Normalize(end - start);
	LineSegment segment = LineSegment(start, start + 1000 * segmentDirection);
	CastInfo castInfo;

	bool segmentCastResult = SegmentCast(mGame->GetColliders(), segment, castInfo);

	Portal* orangePortal = mGame->GetOrangePortal();
	Portal* bluePortal = mGame->GetBluePortal();

	if (segmentCastResult)
	{

		Block* block = dynamic_cast<Block*>(castInfo.mActor);
		//Not a block
		if (block == nullptr)
		{
			mAudio->PlaySound("PortalFail.ogg");
		}
		//A block
		else
		{
			Portal* portal = new Portal(mGame);
			portal->SetPosition(castInfo.mPoint);

			Vector3 desiredFacing = castInfo.mNormal;
			Vector3 originalFacing = Vector3::UnitX;
			Quaternion result;

			float dotProduct = originalFacing.Dot(originalFacing, desiredFacing);

			if (dotProduct == 1)
			{
				result = Quaternion::Identity;
			}
			else if (dotProduct == -1)
			{
				result = Quaternion(Vector3::UnitZ, Math::Pi);
			}
			else
			{
				//normalized axis of rotation
				Vector3 axisRotation = desiredFacing.Cross(originalFacing, desiredFacing);
				Vector3 normalizedAxisRotation = axisRotation.Normalize(axisRotation);

				//angle of rotation
				float theta = Math::Acos(dotProduct);

				result = Quaternion(normalizedAxisRotation, theta);
			}

			if (std::abs(castInfo.mNormal.x) == 1)
			{
				CollisionComponent* cc = new CollisionComponent(portal);
				cc->SetSize(110.0f, 125.0f, 10.0f);
				portal->SetCollisionComponent(cc);
			}
			else if (std::abs(castInfo.mNormal.y) == 1)
			{
				CollisionComponent* cc = new CollisionComponent(portal);
				cc->SetSize(10.0f, 125.0f, 110.0f);
				portal->SetCollisionComponent(cc);
			}
			else
			{
				CollisionComponent* cc = new CollisionComponent(portal);
				cc->SetSize(110.0f, 10.0f, 125.0f);
				portal->SetCollisionComponent(cc);
			}

			if (!isBlue)
			{
				if (orangePortal != nullptr)
				{
					orangePortal->SetState(ActorState::Destroy);
				}
				portal->GetComponent<MeshComponent>()->SetTextureIndex(1);
				mGame->SetOrangePortal(portal);
				if (bluePortal == nullptr)
				{
					mCrosshair->SetState(CrosshairState::OrangeFill);
				}
				else
				{
					mCrosshair->SetState(CrosshairState::BothFill);
				}
				mAudio->PlaySound("PortalShootOrange.ogg");
				mGame->GetOrangePortal()->SetQuat(result);
			}
			else
			{
				if (bluePortal != nullptr)
				{
					bluePortal->SetState(ActorState::Destroy);
				}
				if (orangePortal == nullptr)
				{
					mCrosshair->SetState(CrosshairState::BlueFill);
				}
				else
				{
					mCrosshair->SetState(CrosshairState::BothFill);
				}
				mGame->SetBluePortal(portal);
				mAudio->PlaySound("PortalShootBlue.ogg");
				mGame->GetBluePortal()->SetQuat(result);
			}
		}
	}
	else
	{
		mAudio->PlaySound("PortalFail.ogg");
	}
}

void PlayerMove::PhysicsUpdate(float deltaTime)
{
	mAcceleration = mPendingForces * (1.0f / mMass);
	mVelocity += mAcceleration * deltaTime;
	FixXYVelocity();

	if (mVelocity.z < -1000.0)
	{
		mVelocity.z = -1000.0f;
	}

	mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);
	mOwner->SetRotation(mOwner->GetRotation() + mAngularSpeed * deltaTime);
	mPendingForces = Vector3::Zero;
}

void PlayerMove::UpdateOnGround(float deltaTime)
{
	PhysicsUpdate(deltaTime);
	bool teleport = UpdatePortalTeleport(deltaTime);
	if (teleport)
	{
		return;
	}
	std::vector<Actor*> colliders = mGame->GetColliders();
	bool falling = true;
	for (int i = 0; i < colliders.size(); i++)
	{
		CollSide result = FixCollision(mOwner->GetComponent<CollisionComponent>(),
									   colliders[i]->GetComponent<CollisionComponent>());

		if (result == CollSide::Top)
		{
			falling = false;
		}
	}

	if (falling)
	{
		ChangeState(Falling);
	}
}

void PlayerMove::UpdateJump(float deltaTime)
{
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	bool teleport = UpdatePortalTeleport(deltaTime);
	if (teleport)
	{
		return;
	}
	std::vector<Actor*> colliders = mGame->GetColliders();
	for (int i = 0; i < colliders.size(); i++)
	{
		CollSide result = FixCollision(mOwner->GetComponent<CollisionComponent>(),
									   colliders[i]->GetComponent<CollisionComponent>());

		if (result == CollSide::Bottom)
		{
			mVelocity.z = 0.0f;
		}
	}

	if (mVelocity.z <= 0.0f)
	{
		ChangeState(Falling);
	}
}

void PlayerMove::UpdateFalling(float deltaTime)
{
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	bool teleport = UpdatePortalTeleport(deltaTime);
	if (teleport)
	{
		return;
	}
	std::vector<Actor*> colliders = mGame->GetColliders();
	bool landed = false;
	for (int i = 0; i < colliders.size(); i++)
	{
		CollSide result = FixCollision(mOwner->GetComponent<CollisionComponent>(),
									   colliders[i]->GetComponent<CollisionComponent>());

		if ((result == CollSide::Top) && (mVelocity.z <= 0.0f))
		{
			landed = true;
		}
	}
	if (landed)
	{
		mVelocity.z = 0;
		//when falling changes to ground, change above bool back to false
		ChangeState(OnGround);
		mTeleporting = false;
		mAudio->PlaySound("Land.ogg");
	}
}

void PlayerMove::FixXYVelocity()
{
	Vector2 xyVelocity(mVelocity.x, mVelocity.y);
	if (xyVelocity.Length() > MAX_SPEED)
	{
		xyVelocity = xyVelocity.Normalize(xyVelocity);
		xyVelocity = MAX_SPEED * xyVelocity;
	}

	//make sure if falling (via bool) don't limit velocity in fixxyvelocity
	if (mCurrentState == OnGround && !mTeleporting)
	{
		if ((Math::NearZero(mAcceleration.x)) || (mAcceleration.x > 0 && xyVelocity.x < 0) ||
			(mAcceleration.x < 0 && xyVelocity.x > 0))
		{
			xyVelocity.x *= 0.9f;
		}

		if ((Math::NearZero(mAcceleration.y)) || (mAcceleration.y > 0 && xyVelocity.y < 0) ||
			(mAcceleration.y < 0 && xyVelocity.y > 0))
		{
			xyVelocity.y *= 0.9f;
		}
	}

	mVelocity.x = xyVelocity.x;
	mVelocity.y = xyVelocity.y;
}

void PlayerMove::ProcessInput(const Uint8* keyState, Uint32 mouseButtons,
							  const Vector2& relativeMouse)
{

	if (mOwner->GetComponent<HealthComponent>()->IsDead())
	{
		return;
	}

	if (!mLastFrameSpace && keyState[SDL_SCANCODE_SPACE] && mCurrentState == OnGround)
	{
		AddForce(mJumpForce);
		ChangeState(Jump);
		mAudio->PlaySound("Jump.ogg");
	}

	if (!mLastFrameR && keyState[SDL_SCANCODE_R])
	{
		Portal* blue = mGame->GetBluePortal();
		Portal* orange = mGame->GetOrangePortal();

		if (blue != nullptr)
		{
			blue->SetState(ActorState::Destroy);
		}
		if (orange != nullptr)
		{
			orange->SetState(ActorState::Destroy);
		}
		mGame->SetBluePortal(nullptr);
		mGame->SetOrangePortal(nullptr);
		mCrosshair->SetState(CrosshairState::Default);
		mAudio->PlaySound("PortalClose.ogg");
	}

	if (mGame->GetPlayer()->HasGun())
	{
		if (!mLastFrameLeft && (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)))
		{
			CreatePortal(true);
		}
		if (!mLastFrameRight && (mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT)))
		{
			CreatePortal(false);
		}
	}

	//w sets adds force in owner's forward
	if (keyState[SDL_SCANCODE_W])
	{
		AddForce(mOwner->GetForward() * 700.0f);
	}
	//s adds force in opposite direction to forward
	if (keyState[SDL_SCANCODE_S])
	{
		AddForce(mOwner->GetForward() * -700.0f);
	}
	//d adds force in owner's right
	if (keyState[SDL_SCANCODE_D])
	{
		AddForce(mOwner->GetRight() * 700.0f);
	}
	//a adds force in opposite direction to right
	if (keyState[SDL_SCANCODE_A])
	{
		AddForce(mOwner->GetRight() * -700.0f);
	}

	float angularSpeed = relativeMouse.x / 500.0f * Math::Pi * 10.0f;
	float pitchSpeed = relativeMouse.y / 500.0f * Math::Pi * 10.0f;
	SetAngularSpeed(angularSpeed);
	mOwner->GetComponent<CameraComponent>()->SetPitchSpeed(pitchSpeed);
	mLastFrameSpace = keyState[SDL_SCANCODE_SPACE];
	mLastFrameR = keyState[SDL_SCANCODE_R];
	mLastFrameLeft = mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT);
	mLastFrameRight = mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT);
}

CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* collider)
{
	Vector3 offset = Vector3::Zero;
	CollSide collision = self->GetMinOverlap(collider, offset);

	if (collision != CollSide::None)
	{
		mOwner->SetPosition(mOwner->GetPosition() + offset);
	}

	if (collision == CollSide::Left)
	{
		Vector3 normal(0, -1, 0);
		AddForce(normal * NORMAL_FORCE);
	}
	else if (collision == CollSide::Right)
	{
		Vector3 normal(0, 1, 0);
		AddForce(normal * NORMAL_FORCE);
	}
	else if (collision == CollSide::Front)
	{
		Vector3 normal(1, 0, 0);
		AddForce(normal * NORMAL_FORCE);
	}
	else if (collision == CollSide::Back)
	{
		Vector3 normal(-1, 0, 0);
		AddForce(normal * NORMAL_FORCE);
	}

	return collision;
}
