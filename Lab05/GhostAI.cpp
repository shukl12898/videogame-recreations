#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"
#include "Math.h"
#include <unordered_set>

GhostAI::GhostAI(class Actor* owner)
: Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
	mCollisionComponent = new CollisionComponent(mOwner);
	mCollisionComponent->SetSize(1.0f, 1.0f);
}

void GhostAI::UpdateDirection()
{
	mDirection = (mNextNode->GetPosition() - mPrevNode->GetPosition());
	mDirection.Normalize();

	//Moving right
	if (mDirection.x > 0)
	{
		mOwner->GetComponent<AnimatedSprite>()->SetAnimation("right");
	}
	//Moving left
	if (mDirection.x < 0)
	{
		mOwner->GetComponent<AnimatedSprite>()->SetAnimation("left");
	}
	//Moving down
	if (mDirection.y > 0)
	{
		mOwner->GetComponent<AnimatedSprite>()->SetAnimation("down");
	}
	//Moving up
	if (mDirection.y < 0)
	{
		mOwner->GetComponent<AnimatedSprite>()->SetAnimation("up");
	}
}

void GhostAI::Update(float deltaTime)
{

	mTimeSpent += deltaTime;

	if (mState == Scatter || mState == Chase)
	{
		Vector2 newPos(mOwner->GetPosition() + SCATTER_SPEED * mDirection * deltaTime);
		mOwner->SetPosition(newPos);
	}
	else if (mState == Frightened)
	{
		Vector2 newPos(mOwner->GetPosition() + FRIGHTENED_SPEED * mDirection * deltaTime);
		mOwner->SetPosition(newPos);
	}
	else if (mState == Dead)
	{
		Vector2 newPos(mOwner->GetPosition() + DEAD_SPEED * mDirection * deltaTime);
		mOwner->SetPosition(newPos);
	}

	// check if ghost intersects with nxet node
	if (mCollisionComponent->Intersect(mNextNode->GetComponent<CollisionComponent>()))
	{
		//Set the ghost's position to mNextNode's position
		mOwner->SetPosition(mNextNode->GetPosition());

		//Check if you need to change the state (not necessary for now)
		StateChange();

		//Update target node as needed (seperate function)
		UpdateTargetNode();

		//Updated mPrevNode and mNextNode (seperate function)
		UpdateOtherNodes();
		UpdateDirection();
	}

	if (mState == Frightened)
	{
		AnimatedSprite* animatedSprite = mOwner->GetComponent<AnimatedSprite>();
		if (mTimeSpent <= 5)
		{
			animatedSprite->SetAnimation("scared0");
		}
		else
		{
			animatedSprite->SetAnimation("scared1");
		}
	}
}

void GhostAI::StateChange()
{
	if (mTimeSpent >= 7 && mState == Frightened)
	{
		mState = Scatter;
		mTimeSpent = 0.0f;
	}
}

void GhostAI::UpdateTargetNode()
{
	if (mState == Scatter)
	{
		mTargetNode = mGhost->GetScatterNode();
	}
	else if (mState == Frightened)
	{
		std::unordered_set<PathNode*> firstPriorityNodes;
		std::unordered_set<PathNode*> secondPriorityNodes;

		for (auto adjNode : mNextNode->mAdjacent)
		{
			PathNode::Type nodeType = adjNode->GetType();
			if (adjNode != mPrevNode && nodeType != PathNode::Ghost && nodeType != PathNode::Tunnel)
			{
				firstPriorityNodes.insert(adjNode);
			}
			if (adjNode != mPrevNode && nodeType != PathNode::Tunnel)
			{
				secondPriorityNodes.insert(adjNode);
			}
		}

		PathNode* selectedNode = nullptr;

		if (!firstPriorityNodes.empty())
		{
			selectedNode = *(firstPriorityNodes.begin());
		}
		else if (!secondPriorityNodes.empty())
		{
			selectedNode = *(secondPriorityNodes.begin());
		}
		else
		{
			int index = Random::GetIntRange(0, mNextNode->mAdjacent.size() - 1);
			selectedNode = mNextNode->mAdjacent.at(index);
		}

		mTargetNode = selectedNode;
	}
}

void GhostAI::UpdateOtherNodes()
{

	//select a node adjacent to mNextNode closest to mTargetNode (loop through mAdjacent vector)
	//cant pick mPrevNode
	//any node with GetType() PathNode::Ghost
	//any node with GetType() PathNode::Tunnel

	float minDist = INFINITY;
	PathNode* selectedNode = nullptr;

	for (auto adjNode : mNextNode->mAdjacent)
	{
		PathNode::Type nodeType = adjNode->GetType();
		if ((adjNode != mPrevNode) && (nodeType != PathNode::Ghost) &&
			(nodeType != PathNode::Tunnel))
		{
			float currDist =
				adjNode->GetPosition().Distance(adjNode->GetPosition(), mTargetNode->GetPosition());
			if (currDist <= minDist)
			{
				selectedNode = adjNode;
				minDist = currDist;
			}
		}
	}

	if (selectedNode == nullptr)
	{
		for (auto adjNode : mNextNode->mAdjacent)
		{
			PathNode::Type nodeType = adjNode->GetType();
			if ((adjNode != mPrevNode) && (nodeType != PathNode::Tunnel))
			{
				float currDist = adjNode->GetPosition().Distance(adjNode->GetPosition(),
																 mTargetNode->GetPosition());
				if (currDist <= minDist)
				{
					selectedNode = adjNode;
					minDist = currDist;
				}
			}
		}
	}

	if (selectedNode == nullptr)
	{
		for (auto adjNode : mNextNode->mAdjacent)
		{
			PathNode::Type nodeType = adjNode->GetType();
			if (nodeType != PathNode::Tunnel)
			{
				float currDist = adjNode->GetPosition().Distance(adjNode->GetPosition(),
																 mTargetNode->GetPosition());
				if (currDist <= minDist)
				{
					selectedNode = adjNode;
					minDist = currDist;
				}
			}
		}
	}

	mPrevNode = mNextNode;
	mNextNode = selectedNode;
}

void GhostAI::Frighten()
{
	if (mState != Dead)
	{
		mTimeSpent = 0.0f;
		if (mState != Frightened)
		{
			mState = Frightened;
			PathNode* temp = mPrevNode;
			mPrevNode = mNextNode;
			mNextNode = temp;
			UpdateDirection();
			mTargetNode = nullptr;
		}
	}
}

void GhostAI::Start(PathNode* startNode)
{
	mOwner->SetPosition(startNode->GetPosition());
	mState = Scatter;
	mPrevNode = nullptr;
	mNextNode = startNode;
	//Make sure to reset to 0.0f for every time state changes
	mTimeSpent = 0.0f;
}

void GhostAI::Die()
{
	// TODO: Implement
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render, static_cast<int>(mOwner->GetPosition().x),
						   static_cast<int>(mOwner->GetPosition().y),
						   static_cast<int>(mNextNode->GetPosition().x),
						   static_cast<int>(mNextNode->GetPosition().y));
	}
}
