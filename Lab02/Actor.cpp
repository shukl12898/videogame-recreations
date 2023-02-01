#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
: mGame(game)
, mState(ActorState::Active)
, mPosition(Vector2::Zero)
, mScale(1.0f)
, mRotation(0.0f)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
	for (Component* i : mComponents)
	{
		delete i;
	}
	mComponents.clear();
}

void Actor::Update(float deltaTime)
{
	//Updates all actors (and all subclasses) if actor is active
	if (mState == ActorState::Active)
	{
		for (Component* i : mComponents)
		{
			i->Update(deltaTime);
		}

		OnUpdate(deltaTime);
	}
}

Vector2 Actor::GetForward() const
{
	//Uses rotation angle to get the x and y coordinates of forward vector
	Vector2 forward(cos(mRotation), -sin(mRotation));
	return forward;
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	//Processes input of all actors (and all subclasses) if actor is active
	if (mState == ActorState::Active)
	{
		for (Component* i : mComponents)
		{
			i->ProcessInput(keyState);
		}

		OnProcessInput(keyState);
	}
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}
