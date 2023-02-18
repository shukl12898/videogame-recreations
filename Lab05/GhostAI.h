#pragma once
#include "Component.h"
#include "Math.h"
#include <vector>

class GhostAI : public Component
{
public:
	// Used to track the four different GhostAI states
	enum State
	{

		// Ghost paths from current position to home node ('scatter node'), then continues circling around scatter node until state change
		Scatter,
		// Ghost paths to a designated target node, typically relative to pacman position
		Chase,
		// Ghost turns blue and picks random node to turn to at every intersection
		Frightened,
		// Ghost turns into eyes and paths back to home pen area, at which point it will come back to life
		Dead
	};

	GhostAI(class Actor* owner);

	void Update(float deltaTime) override;

	// Called when the Ghost starts at the beginning
	// (or when the ghosts should respawn)
	void Start(class PathNode* startNode);

	// Get the current state
	State GetState() const { return mState; }

	// Called when the ghost should switch to the "Frightened" state
	void Frighten();

	// Called when the ghost should switch to the "Dead" state
	void Die();

	//  Helper function to draw GhostAI's current goal, prev, and next
	void DebugDrawPath(struct SDL_Renderer* render);

private:
	// Member data for pathfinding
	const int SCATTER_SPEED = 90;
	const int FRIGHTENED_SPEED = 65;
	const int DEAD_SPEED = 125;

	float mTimeSpent = 0.0f;

	// TargetNode is our current goal node
	class PathNode* mTargetNode = nullptr;
	// PrevNode is the last node we intersected
	// with prior to the current position
	class PathNode* mPrevNode = nullptr;
	// NextNode is the next node we're trying to get to
	class PathNode* mNextNode = nullptr;

	// Current state of the Ghost AI
	State mState = Scatter;

	// Save the owning actor (cast to a Ghost*)
	class Ghost* mGhost;
	class CollisionComponent* mCollisionComponent;

	// TODO: Add any member data/helper functions here!
	Vector2 mDirection;

	void UpdateTargetNode();
	void StateChange();
	void UpdateDirection();
	void UpdateOtherNodes();
	class CollisionComponent* GetCollisionComponent() const { return mCollisionComponent; }
};
