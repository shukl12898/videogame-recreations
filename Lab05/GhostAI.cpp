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
	AnimatedSprite* animatedSprite = mOwner->GetComponent<AnimatedSprite>();

	//Moving right
	if (mDirection.x > 0)
	{
		animatedSprite->SetAnimation("right");
		if (mState == Dead)
		{
			animatedSprite->SetAnimation("deadright");
		}
	}
	//Moving left
	if (mDirection.x < 0)
	{
		animatedSprite->SetAnimation("left");
		if (mState == Dead)
		{
			animatedSprite->SetAnimation("deadleft");
		}
	}
	//Moving down
	if (mDirection.y > 0)
	{
		animatedSprite->SetAnimation("down");
		if (mState == Dead)
		{
			animatedSprite->SetAnimation("deaddown");
		}
	}
	//Moving up
	if (mDirection.y < 0)
	{
		animatedSprite->SetAnimation("up");
		if (mState == Dead)
		{
			animatedSprite->SetAnimation("deadright");
		}
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
	if (mState == Dead)
	{
		Vector2 ghostPen = mOwner->GetGame()->GetGhostPen()->GetPosition();
		bool xCoordinate = mOwner->GetPosition().x == ghostPen.x;
		bool yCoordinate = mOwner->GetPosition().y == ghostPen.y;
		if (xCoordinate && yCoordinate)
		{
			mState = Scatter;
			mTimeSpent = 0.0f;
		}
	}
	if (mState == Scatter && mTimeSpent >= 5)
	{
		mState = Chase;
		mTimeSpent = 0.0f;
	}
	if (mState == Chase && mTimeSpent >= 20)
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
	else if (mState == Dead)
	{
		mTargetNode = mOwner->GetGame()->GetGhostPen();
	}
	else if (mState == Chase)
	{
		//Blinky targets pacman's previous path node
		if (mGhost->GetType() == Ghost::Type::Blinky)
		{
			PathNode* pacPrev = mOwner->GetGame()->GetPlayer()->GetPrevNode();
			if (pacPrev->GetType() == PathNode::Tunnel)
			{
				float minDist = INFINITY;
				PathNode* selectedNode = nullptr;

				for (auto adjNode : pacPrev->mAdjacent)
				{
					if (adjNode->GetType() == PathNode::Default)
					{
						float currDist = adjNode->GetPosition().Distance(adjNode->GetPosition(),
																		 pacPrev->GetPosition());
						if (currDist <= minDist)
						{
							selectedNode = adjNode;
							minDist = currDist;
						}
					}
				}
				mTargetNode = selectedNode;
			}
			else
			{
				mTargetNode = pacPrev;
			}
		}
		//Pinky gets a point 80 units 'in front of pacman'
		else if (mGhost->GetType() == Ghost::Type::Pinky)
		{
			Vector2 inFront = mOwner->GetGame()->GetPlayer()->GetPointInFrontOf(80);

			float minDist = INFINITY;
			PathNode* selectedNode = nullptr;

			for (auto adjNode : mOwner->GetGame()->GetPathNodes())
			{
				if (adjNode->GetType() == PathNode::Default)
				{
					float currDist =
						adjNode->GetPosition().Distance(adjNode->GetPosition(), inFront);
					if (currDist <= minDist)
					{
						selectedNode = adjNode;
						minDist = currDist;
					}
				}
			}
			mTargetNode = selectedNode;
		}
		//Inky gets a point 40 units 'in front of pacman'
		else if (mGhost->GetType() == Ghost::Type::Inky)
		{
			Vector2 P = mOwner->GetGame()->GetPlayer()->GetPointInFrontOf(40);
			Vector2 v = P - mOwner->GetGame()->GetGhosts().at(0)->GetPosition();
			v *= 2;
			Vector2 q = mOwner->GetGame()->GetGhosts().at(0)->GetPosition() + v;

			float minDist = INFINITY;
			PathNode* selectedNode = nullptr;

			for (auto adjNode : mOwner->GetGame()->GetPathNodes())
			{
				if (adjNode->GetType() == PathNode::Default)
				{
					float currDist = adjNode->GetPosition().Distance(adjNode->GetPosition(), q);
					if (currDist <= minDist)
					{
						selectedNode = adjNode;
						minDist = currDist;
					}
				}
			}
			mTargetNode = selectedNode;
		}
		//Clyde gets distance between clyde and player (if distance > 150), targets pacman's previous or scatter
		else if (mGhost->GetType() == Ghost::Type::Clyde)
		{
			float currDist = mOwner->GetPosition().Distance(
				mOwner->GetPosition(), mOwner->GetGame()->GetPlayer()->GetPosition());

			if (currDist > 150.0f)
			{
				PathNode* pacPrev = mOwner->GetGame()->GetPlayer()->GetPrevNode();
				if (pacPrev->GetType() == PathNode::Tunnel)
				{
					float minDist = INFINITY;
					PathNode* selectedNode = nullptr;

					for (auto adjNode : pacPrev->mAdjacent)
					{
						if (adjNode->GetType() == PathNode::Default)
						{
							float currDist = adjNode->GetPosition().Distance(
								adjNode->GetPosition(), pacPrev->GetPosition());
							if (currDist <= minDist)
							{
								selectedNode = adjNode;
								minDist = currDist;
							}
						}
					}
					mTargetNode = selectedNode;
				}
				else
				{
					mTargetNode = pacPrev;
				}
			}
			else
			{
				mTargetNode = mGhost->GetScatterNode();
			}
		}
	}
}

void GhostAI::UpdateOtherNodes()
{
	float minDist = INFINITY;
	PathNode* selectedNode = nullptr;

	for (auto adjNode : mNextNode->mAdjacent)
	{
		PathNode::Type nodeType = adjNode->GetType();
		if (((adjNode != mPrevNode) && (nodeType != PathNode::Ghost) &&
			 (nodeType != PathNode::Tunnel)) ||
			(nodeType == PathNode::Ghost && mState == Dead))
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
	mTimeSpent = 0.0f;
	mState = Dead;
	UpdateDirection();
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
