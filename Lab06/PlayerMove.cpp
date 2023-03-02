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

	for (EnemyComponent* enemy : mGame->GetEnemyComponents())
	{
		Vector2 offset(0, 0);

		if (mAttack)
		{
			CollisionComponent* swordCC = mSword->GetComponent<CollisionComponent>();
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

	Vector2 offset(-256.0f, -224.0f);
	mGame->SetCameraPos(mOwner->GetPosition() + offset);
}

void PlayerMove::SwordUpdate()
{
	Vector2 ownerPos = mOwner->GetPosition();

	if (mDirectionState == DirectionState::Up)
	{
		Vector2 offset(0, -40);
		mSword->GetComponent<CollisionComponent>()->SetSize(20, 28);
		mSword->SetPosition(ownerPos + offset);
	}
	else if (mDirectionState == DirectionState::Down)
	{
		Vector2 offset(0, 40);
		mSword->GetComponent<CollisionComponent>()->SetSize(20, 28);
		mSword->SetPosition(ownerPos + offset);
	}
	else if (mDirectionState == DirectionState::Left)
	{
		Vector2 offset(-32, 0);
		mSword->GetComponent<CollisionComponent>()->SetSize(28, 20);
		mSword->SetPosition(ownerPos + offset);
	}
	else if (mDirectionState == DirectionState::Right)
	{
		Vector2 offset(32, 0);
		mSword->GetComponent<CollisionComponent>()->SetSize(28, 20);
		mSword->SetPosition(ownerPos + offset);
	}
}

const std::string PlayerMove::DetermineAnimation()
{
	std::string animName;

	if (mMoving)
	{
		if (mDirectionState == DirectionState::Up)
		{
			animName = "WalkUp";
		}
		else if (mDirectionState == DirectionState::Down)
		{
			animName = "WalkDown";
		}
		else if (mDirectionState == DirectionState::Left)
		{
			animName = "WalkLeft";
		}
		else if (mDirectionState == DirectionState::Right)
		{
			animName = "WalkRight";
		}
	}
	else if (mAttack)
	{
		if (mDirectionState == DirectionState::Up)
		{
			animName = "AttackUp";
		}
		else if (mDirectionState == DirectionState::Down)
		{
			animName = "AttackDown";
		}
		else if (mDirectionState == DirectionState::Left)
		{
			animName = "AttackLeft";
		}
		else if (mDirectionState == DirectionState::Right)
		{
			animName = "AttackRight";
		}
	}
	else
	{
		if (mDirectionState == DirectionState::Up)
		{
			animName = "StandUp";
		}
		else if (mDirectionState == DirectionState::Down)
		{
			animName = "StandDown";
		}
		else if (mDirectionState == DirectionState::Left)
		{
			animName = "StandLeft";
		}
		else if (mDirectionState == DirectionState::Right)
		{
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
		mAttackTime = 0.25;
		mOwner->GetAnimatedSprite()->ResetAnimTimer();
	}
	else if (keyState[SDL_SCANCODE_W])
	{
		mDirection = Vector2(0, -1);
		mDirectionState = DirectionState::Up;
		mMoving = true;
	}
	//facing and moving down
	else if (keyState[SDL_SCANCODE_S])
	{
		mDirection = Vector2(0, 1);
		mDirectionState = DirectionState::Down;
		mMoving = true;
	}
	//facing and moving left
	else if (keyState[SDL_SCANCODE_A])
	{
		mDirection = Vector2(-1, 0);
		mDirectionState = DirectionState::Left;
		mMoving = true;
	}
	//facing and moving right
	else if (keyState[SDL_SCANCODE_D])
	{
		mDirection = Vector2(1, 0);
		mDirectionState = DirectionState::Right;
		mMoving = true;
	}
	else
	{
		mDirection = Vector2(0, 0);
		mMoving = false;
	}

	mLastFrame = keyState[SDL_SCANCODE_SPACE];
}