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
	mDirectionState = DirectionState::Down;
}

void PlayerMove::Update(float deltaTime)
{

	if (mAttack && mAttackTime >= 0)
	{

		SwordUpdate();
		mAttackTime -= deltaTime;
		if (!mSwordSound)
		{
			mGame->GetAudioSystem()->PlaySound("SwordSlash.wav", false);
			mSwordSound = true;
		}
	}
	else
	{
		mAttack = false;
		mSwordSound = false;
	}

	if (mMoving)
	{
		mOwner->SetPosition(mOwner->GetPosition() + mDirection * deltaTime * MOVE_SPEED);
	}

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
			break;
		case DirectionState::Down:
			animName = "WalkDown";
			break;
		case DirectionState::Left:
			animName = "WalkLeft";
			break;
		case DirectionState::Right:
			animName = "WalkRight";
			break;
		}
	}

	if (mAttack)
	{
		switch (mDirectionState)
		{
		case DirectionState::Up:
			animName = "AttackUp";
			break;
		case DirectionState::Down:
			animName = "AttackDown";
			break;
		case DirectionState::Left:
			animName = "AttackLeft";
			break;
		case DirectionState::Right:
			animName = "AttackRight";
			break;
		}
	}

	if (!mMoving && !mAttack)
	{
		switch (mDirectionState)
		{
		case DirectionState::Up:
			animName = "StandUp";
			break;
		case DirectionState::Down:
			animName = "StandDown";
			break;
		case DirectionState::Left:
			animName = "StandLeft";
			break;
		case DirectionState::Right:
			animName = "StandRight";
			break;
		}
	}

	return animName;
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{

	//attack
	if (!mLastFrame && keyState[SDL_SCANCODE_SPACE] && mAttackTime <= 0)
	{
		mAttack = true;
		mMoving = false;
		mAttackTime = ATTACK_DURATION;
		mOwner->GetAnimatedSprite()->ResetAnimTimer();
	}

	//facing and moving up
	if (keyState[SDL_SCANCODE_W] && !mAttack)
	{
		mDirection = Vector2::NegUnitY;
		mDirectionState = DirectionState::Up;
		mMoving = true;
	}
	//facing and moving down
	if (keyState[SDL_SCANCODE_S] && !mAttack)
	{
		mDirection = Vector2::UnitY;
		mDirectionState = DirectionState::Down;
		mMoving = true;
	}
	//facing and moving left
	if (keyState[SDL_SCANCODE_A] && !mAttack)
	{
		mDirection = Vector2::NegUnitX;
		mDirectionState = DirectionState::Left;
		mMoving = true;
	}
	//facing and moving right
	if (keyState[SDL_SCANCODE_D] && !mAttack)
	{
		mDirection = Vector2::UnitX;
		mDirectionState = DirectionState::Right;
		mMoving = true;
	}

	if (!(keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_A] ||
		  keyState[SDL_SCANCODE_D]))
	{
		mDirection = Vector2::Zero;
		mMoving = false;
	}

	mLastFrame = keyState[SDL_SCANCODE_SPACE];
}