#pragma once
#include "Actor.h"

class Enemy : public Actor
{
public:
	Enemy(class Game* game);

private:
	class MeshComponent* mMeshComp;
	class EnemyMove* mEnemyMove;
};