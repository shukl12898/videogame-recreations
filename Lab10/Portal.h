#pragma once
#include "Actor.h"

class Portal : public Actor
{
public:
	Portal(class Game* game);
	void SetCollisionComponent(class CollisionComponent* cc) { mCollisionComponent = cc; };
	void OnUpdate(float deltaTime) override;
	Matrix4 CalcViewMatrixBasic(Portal* other);
	float GetOutYaw() const { return mOutYaw; };
	Matrix4 CalcViewMatrix(Portal* other);
	Vector3 GetOut(Vector3 playerToIn, Portal* other);

private:
	class PortalMeshComponent* mPortalMesh = nullptr;
	class CollisionComponent* mCollisionComponent = nullptr;
	Matrix4 mComplexView;
	float mOutYaw = 0.0f;
};