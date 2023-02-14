#include "GoombaMove.h"
#include "Player.h"
#include "Goomba.h"
#include "Game.h"
#include "Block.h"
#include "MoveComponent.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Math.h"

GoombaMove::GoombaMove(Goomba* owner)
: MoveComponent(owner)
{

	mGame = owner->GetGame();
	mOwner = owner;
	mForwardSpeed = -100;
}

void GoombaMove::Update(float deltaTime)
{
	Vector2 newPos(mOwner->GetPosition().x + mForwardSpeed * deltaTime,
				   mOwner->GetPosition().y + mYSpeed * deltaTime);

	CollisionComponent* collComponent = mOwner->GetCollisionComponent();

	if (newPos.y > 448.0f)
	{
		mOwner->SetState(ActorState::Destroy);
	}

	mOwner->SetPosition(newPos);

	for (Block* block : mGame->GetBlocks())
	{
		Vector2 offset(0, 0);
		CollSide result = collComponent->GetMinOverlap(block->GetCollisionComponent(), offset);

		if (result == CollSide::Top && (mYSpeed) > 0.0f)
		{
			mYSpeed = 0;
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}

	for (Block* block : mGame->GetBlocks())
	{
		Vector2 offset(0, 0);
		CollSide result = collComponent->GetMinOverlap(block->GetCollisionComponent(), offset);

		if (result == CollSide::Left &&
			(abs(block->GetPosition().y - mOwner->GetPosition().y) <= 3))
		{
			mForwardSpeed = -GOOMBA_SPEED;
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}

		if (result == CollSide::Right &&
			(abs(block->GetPosition().y - mOwner->GetPosition().y) <= 3))
		{
			mForwardSpeed = GOOMBA_SPEED;
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}

	for (Goomba* goomba : mGame->GetGoombas())
	{
		if (goomba != mOwner)
		{

			Vector2 offset(0, 0);
			CollSide result = collComponent->GetMinOverlap(goomba->GetCollisionComponent(), offset);

			if (result == CollSide::Right)
			{
				mForwardSpeed = GOOMBA_SPEED;
				mOwner->SetPosition(mOwner->GetPosition() + offset);
			}
			else if (result == CollSide::Left)
			{
				mForwardSpeed = -GOOMBA_SPEED;
				mOwner->SetPosition(mOwner->GetPosition() + offset);
			}
		}
	}

	if (mOwner->GetStomped())
	{
		mOwner->GetAnimatedSprite()->SetAnimation("dead");
		mForwardSpeed = 0;
	}

	mYSpeed += GRAVITY * deltaTime;
}