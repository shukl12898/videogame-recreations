#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include "CollisionComponent.h"

class PlayerMove : public MoveComponent
{
public:
	enum MoveState
	{
		OnGround,
		Jump,
		Falling
	};

	PlayerMove(class Actor* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState, Uint32 mouseButtons,
					  const Vector2& relativeMouse) override;
	const float MAX_SPEED = 400.0f;
	const float NORMAL_FORCE = 700.0f;

private:
	void ChangeState(MoveState state) { mCurrentState = state; };
	void UpdateOnGround(float deltaTime);
	void UpdateJump(float deltaTime);
	void UpdateFalling(float deltaTime);
	bool UpdatePortalTeleport(float deltaTime);
	void CreatePortal(bool isBlue);
	void Teleport(class Portal* entry, class Portal* exit);

	void PhysicsUpdate(float deltaTime);
	void FixXYVelocity();
	void AddForce(const Vector3& force) { mPendingForces += force; };
	CollSide FixCollision(CollisionComponent* self, CollisionComponent* collider);

	Vector3 mVelocity;
	Vector3 mAcceleration;
	Vector3 mPendingForces;
	Vector3 mJumpForce;
	Vector3 mGravity;
	float mMass = 1.0f;
	float mCountdown = 0.0f;

	int mSound = 0;
	class Game* mGame = nullptr;
	bool mLastFrameSpace = false;
	bool mLastFrameR = false;
	int mCurrentState;
	class Crosshair* mCrosshair = nullptr;
	bool mLastFrameLeft = false;
	bool mLastFrameRight = false;
	bool mTeleporting = false;
};