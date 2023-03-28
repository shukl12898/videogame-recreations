#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
: mGame(game)
, mState(ActorState::Active)
, mPosition(Vector3::Zero)
, mScale(1.0f)
, mRotation(0.0f)
, mRollAngle(0.0f)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
	for (auto i : mComponents)
	{
		delete i;
	}
	mComponents.clear();
}

void Actor::Update(float deltaTime)
{
	if (mState == ActorState::Active)
	{
		for (auto i : mComponents)
		{
			i->Update(deltaTime);
		}
		OnUpdate(deltaTime);
	}

	CalcWorldTransform();
}

void Actor::CalcWorldTransform()
{
	Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
	Matrix4 rotationMatrixZ = Matrix4::CreateRotationZ(mRotation);
	Matrix4 translationMatrix = Matrix4::CreateTranslation(mPosition);
	Matrix4 rotationMatrixX = Matrix4::CreateRotationX(mRollAngle);

	mWorldTransform = scaleMatrix * rotationMatrixZ * rotationMatrixX *
					  Matrix4::CreateFromQuaternion(mQuat) * translationMatrix;
}

void Actor::SetScale(float scale)
{
	Vector3 scaleVec(scale, scale, scale);
	mScale = scaleVec;
}

Vector3 Actor::GetForward() const
{
	Vector3 forward(cos(mRotation), sin(mRotation), 0.0f);
	return forward;
}

Vector3 Actor::GetRight() const
{
	Vector3 forward(cos(mRotation + Math::PiOver2), sin(mRotation + Math::PiOver2), 0.0f);
	return forward;
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState, Uint32 mouseButtons, const Vector2& relativeMouse)
{
	if (mState == ActorState::Active)
	{
		for (auto i : mComponents)
		{
			i->ProcessInput(keyState, mouseButtons, relativeMouse);
		}
		OnProcessInput(keyState, mouseButtons, relativeMouse);
	}
}

void Actor::OnProcessInput(const Uint8* keyState, Uint32 mouseButtons, const Vector2& relativeMouse)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}
