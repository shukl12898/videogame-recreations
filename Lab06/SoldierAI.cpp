#include "SoldierAI.h"
#include "Actor.h"
#include "Game.h"
#include "PathFinder.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <algorithm>

SoldierAI::SoldierAI(Actor* owner)
: SpriteComponent(owner)
{
	SetIsVisible(true); // Uncomment to hide debug paths
}

void SoldierAI::Setup(PathNode* start, PathNode* end)
{
	mPatrolStart = start;
	mPatrolEnd = end;
	if (GetOwner()->GetGame()->GetPathFinder()->CalculatePath(mPatrolStart, mPatrolEnd, mPath))
	{
		mPrev = mPatrolStart;
		mNext = mPath.back();
		mPath.pop_back();
		UpdateDirection();
	}
}

void SoldierAI::Stunned()
{
	mStunned = true;
	mStunTime = STUN_DURATION;
}

void SoldierAI::Update(float deltaTime)
{

	if (mStunned && mStunTime > 0)
	{
		mOwner->GetComponent<AnimatedSprite>()->SetIsPaused(true);
		mStunTime -= deltaTime;
	}
	else if (mStunned && mStunTime <= 0)
	{
		mStunned = false;
		mOwner->GetComponent<AnimatedSprite>()->SetIsPaused(false);
	}
	else if (!mStunned)
	{

		Vector2 newPos(mOwner->GetPosition() + SOLDIER_SPEED * mDirection * deltaTime);
		mOwner->SetPosition(newPos);

		if (Vector2::Distance(mNext->GetPosition(), mOwner->GetPosition()) <= 1.0f)
		{
			mOwner->SetPosition(mNext->GetPosition());

			if (!mPath.empty())
			{
				mPrev = mNext;
				mNext = mPath.back();
				mPath.pop_back();
				UpdateDirection();
			}
			else
			{
				Setup(mPatrolEnd, mPatrolStart);
			}
		}
	}

	// TODO: Implement
}

void SoldierAI::UpdateDirection()
{
	mDirection = (mNext->GetPosition() - mPrev->GetPosition());
	mDirection.Normalize();
	AnimatedSprite* animatedSprite = mOwner->GetComponent<AnimatedSprite>();

	//Moving right
	if (mDirection.x > 0)
	{
		animatedSprite->SetAnimation("WalkRight");
	}
	//Moving left
	if (mDirection.x < 0)
	{
		animatedSprite->SetAnimation("WalkLeft");
	}
	//Moving down
	if (mDirection.y > 0)
	{
		animatedSprite->SetAnimation("WalkDown");
	}
	//Moving up
	if (mDirection.y < 0)
	{
		animatedSprite->SetAnimation("WalkUp");
	}
}

// This helper is to just debug draw the soldier's path to visualize it
// (only called if this component is set to visible)
void SoldierAI::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();

	// Draw from current position to next
	SDL_RenderDrawLine(renderer, static_cast<int>(mOwner->GetPosition().x - cameraPos.x),
					   static_cast<int>(mOwner->GetPosition().y - cameraPos.y),
					   static_cast<int>(mNext->GetPosition().x - cameraPos.x),
					   static_cast<int>(mNext->GetPosition().y - cameraPos.y));

	// Draw from next to first node on path
	if (!mPath.empty())
	{
		// Draw from current position to next
		SDL_RenderDrawLine(renderer, static_cast<int>(mNext->GetPosition().x - cameraPos.x),
						   static_cast<int>(mNext->GetPosition().y - cameraPos.y),
						   static_cast<int>(mPath.back()->GetPosition().x - cameraPos.x),
						   static_cast<int>(mPath.back()->GetPosition().y - cameraPos.y));
	}

	// Draw each node on the path
	if (mPath.size() > 1)
	{
		for (size_t i = 0; i < mPath.size() - 1; i++)
		{
			SDL_RenderDrawLine(renderer, static_cast<int>(mPath[i]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i]->GetPosition().y - cameraPos.y),
							   static_cast<int>(mPath[i + 1]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i + 1]->GetPosition().y - cameraPos.y));
		}
	}
}
