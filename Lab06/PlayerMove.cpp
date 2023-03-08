#include "PlayerMove.h"
#include "Player.h"
#include "Actor.h"
#include "Game.h"
#include "MoveComponent.h"
#include "Collider.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Math.h"
#include "EnemyComponent.h"
#include <string>
#include "Sword.h"
#include "AudioSystem.h"

PlayerMove::PlayerMove(Player* owner)
: MoveComponent(owner)
{

	mGame = owner->GetGame();
	mOwner = owner;
	mSword = new Sword(mOwner->GetGame());
}

void PlayerMove::Update(float deltaTime)
{

	if (mAttack && mAttackTime >= 0)
	{

		SwordUpdate();
		mAttackTime -= deltaTime;
		mGame->GetAudioSystem()->PlaySound("SwordSlash.wav", false);
	}
	else
	{
		mAttack = false;
	}

	mOwner->SetPosition(mOwner->GetPosition() + mDirection * deltaTime * MOVE_SPEED);
	mOwner->GetAnimatedSprite()->SetAnimation(DetermineAnimation());

	CollisionComponent* cc = mOwner->GetCollisionComponent();
	CollisionComponent* swordCC = mSword->GetComponent<CollisionComponent>();

	for (EnemyComponent* enemy : mGame->GetEnemyComponents())
	{
		Vector2 offset(0, 0);

		if (mAttack)
		{
			if (swordCC->Intersect(enemy->GetCollisionComponent()))
			{
				enemy->TakeDamage();
			}
		}

		CollSide collSide = cc->GetMinOverlap(enemy->GetCollisionComponent(), offset);

		if (collSide != CollSide::None)
		{
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}

	for (Collider* collider : mGame->GetColliders())
	{
		Vector2 offset(0, 0);
		CollSide collSide = cc->GetMinOverlap(collider->GetCollisionComponent(), offset);

		if (collSide != CollSide::None)
		{
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}

	mGame->SetCameraPos(mOwner->GetPosition() + CAMERA_OFFSET);
}

void PlayerMove::SwordUpdate()
{
	Vector2 ownerPos = mOwner->GetPosition();

	if (mDirectionState == DirectionState::Up)
	{
		Vector2 offset(0, SWORD_UP_OFFSET);
		mSword->GetComponent<CollisionComponent>()->SetSize(SWORD_VERTICAL_WIDTH,
															SWORD_VERTICAL_HEIGHT);
		mSword->SetPosition(ownerPos + offset);
	}
	else if (mDirectionState == DirectionState::Down)
	{
		Vector2 offset(0, SWORD_DOWN_OFFSET);
		mSword->GetComponent<CollisionComponent>()->SetSize(SWORD_VERTICAL_WIDTH,
															SWORD_VERTICAL_HEIGHT);
		mSword->SetPosition(ownerPos + offset);
	}
	else if (mDirectionState == DirectionState::Left)
	{
		Vector2 offset(SWORD_LEFT_OFFSET, 0);
		mSword->GetComponent<CollisionComponent>()->SetSize(SWORD_HORIZONTAL_WIDTH,
															SWORD_HORIZONTAL_HEIGHT);
		mSword->SetPosition(ownerPos + offset);
	}
	else if (mDirectionState == DirectionState::Right)
	{
		Vector2 offset(SWORD_RIGHT_OFFSET, 0);
		mSword->GetComponent<CollisionComponent>()->SetSize(SWORD_HORIZONTAL_WIDTH,
															SWORD_HORIZONTAL_HEIGHT);
		mSword->SetPosition(ownerPos + offset);
	}
}

const std::string PlayerMove::DetermineAnimation() const
{
	std::string animName;

	if (mMoving)
	{
		switch (mDirectionState)
		{
		case DirectionState::Up:
			animName = "WalkUp";
		case DirectionState::Down:
			animName = "WalkDown";
		case DirectionState::Left:
			animName = "WalkLeft";
		case DirectionState::Right:
			animName = "WalkRight";
		}
	}
	else if (mAttack)
	{
		switch (mDirectionState)
		{
		case DirectionState::Up:
			animName = "AttackUp";
		case DirectionState::Down:
			animName = "AttackDown";
		case DirectionState::Left:
			animName = "AttackLeft";
		case DirectionState::Right:
			animName = "AttackRight";
		}
	}
	else
	{
		switch (mDirectionState)
		{
		case DirectionState::Up:
			animName = "StandUp";
		case DirectionState::Down:
			animName = "StandDown";
		case DirectionState::Left:
			animName = "StandLeft";
		case DirectionState::Right:
			animName = "StandRight";
		}
	}

	return animName;
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	//facing and moving up

	if (!mLastFrame && keyState[SDL_SCANCODE_SPACE] && mAttackTime <= 0)
	{
		mAttack = true;
		mAttackTime = 0.25f;
		mOwner->GetAnimatedSprite()->ResetAnimTimer();
	}
	if (keyState[SDL_SCANCODE_W])
	{
		mDirection = Vector2::NegUnitY;
		mDirectionState = DirectionState::Up;
		mMoving = true;
	}
	//facing and moving down
	if (keyState[SDL_SCANCODE_S])
	{
		mDirection = Vector2::UnitY;
		mDirectionState = DirectionState::Down;
		mMoving = true;
	}
	//facing and moving left
	if (keyState[SDL_SCANCODE_A])
	{
		mDirection = Vector2::NegUnitX;
		mDirectionState = DirectionState::Left;
		mMoving = true;
	}
	//facing and moving right
	if (keyState[SDL_SCANCODE_D])
	{
		mDirection = Vector2::UnitX;
		mDirectionState = DirectionState::Right;
		mMoving = true;
	}
	else
	{
		mDirection = Vector2::Zero;
		mMoving = false;
	}

	mLastFrame = keyState[SDL_SCANCODE_SPACE];
}