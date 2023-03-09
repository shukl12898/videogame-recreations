#include "PlayerMove.h"
#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "SideBlock.h"
#include "Random.h"
#include "CollisionComponent.h"
#include "Block.h"
#include "Bullet.h"
#include "Player.h"

PlayerMove::PlayerMove(Actor* owner)
: MoveComponent(owner)
{
	mOwner = owner;
	Vector3 velocity(400.0f, 0.0f, 0.0f);
	mVelocity = velocity;
}

void PlayerMove::Update(float deltaTime)
{

	if (mDamage && mDamageTime > 0)
	{
		mDamageTime -= deltaTime;
	}
	if (mDamage && mDamageTime <= 0)
	{
		mDamage = false;
		mDamageTime = 1.0f;
	}
	if (mShieldLevel == 0)
	{
		mOwner->SetState(ActorState::Paused);
	}

	Vector3 position = mOwner->GetPosition() + (mVelocity * deltaTime);
	position.y = Math::Clamp<float>(position.y, -180, 180);
	position.z = Math::Clamp<float>(position.z, -225, 225);

	Game* game = mOwner->GetGame();
	while (mBlocksEnd - mOwner->GetPosition().x < 4000)
	{
		int sideText = 0;
		int topText = 0;

		if (mBlockNum % 4 == 0)
		{
			sideText = 0;
		}
		else if (mBlockNum % 4 == 1)
		{
			sideText = 1;
		}
		else if (mBlockNum % 4 == 2)
		{
			sideText = 2;
		}
		else if (mBlockNum % 4 == 3)
		{
			sideText = 0;
		}

		if (mBlockNum % 2 == 0)
		{
			topText = 6;
		}
		else
		{
			topText = 7;
		}

		SideBlock* block1 = new SideBlock(game, sideText);
		block1->SetPosition(Vector3(mBlocksEnd + (500), 500, 0));
		block1->SetRotation(Math::Pi);

		SideBlock* block2 = new SideBlock(game, sideText);
		block2->SetPosition(Vector3(mBlocksEnd + (500), -500, 0));

		SideBlock* block3 = new SideBlock(game, 5);
		block3->SetPosition(Vector3(mBlocksEnd + (500), 0, -500));

		SideBlock* block4 = new SideBlock(game, topText);
		block4->SetPosition(Vector3(mBlocksEnd + (500), 0, 500));
		mBlocksEnd += 500;
		mBlockNum++;
	}

	while (mObstaclesEnd - mOwner->GetPosition().x < 4000)
	{
		std::string fileNumber;

		if (mObstacleNum <= 20)
		{
			fileNumber = std::to_string(mObstaclesEnd / 1000);
		}
		else
		{
			fileNumber = std::to_string(Random::GetIntRange(1, 20));
		}

		std::string fileName = "Assets/Blocks/" + fileNumber + ".txt";

		mOwner->GetGame()->LoadBlocks(fileName, mObstaclesEnd);
		mObstaclesEnd += 1000;
		mObstacleNum++;
	}

	mOwner->SetPosition(position);

	Vector3 eye = mOwner->GetPosition() - (mOwner->GetForward() * 300);
	Vector3 target = mOwner->GetPosition() + (mOwner->GetForward() * 20);

	eye.z = 0;

	Matrix4 view = Matrix4::CreateLookAt(eye, target, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(view);

	CollisionComponent* cc = mOwner->GetComponent<CollisionComponent>();
	std::vector<Block*> blocks = mOwner->GetGame()->GetBlocks();

	if (!mDamage)
	{
		for (int i = 0; i < blocks.size(); i++)
		{
			if (cc->Intersect(blocks[i]->GetComponent<CollisionComponent>()))
			{
				blocks[i]->SetState(ActorState::Destroy);
				// Explosion();
				mDamage = true;
			}
		}
	}

	if (mDamage&& mDamageTime == 1.0f)
	{
		mShieldLevel--;
	}
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{

	float forwardSpeed = 0.0f;
	float sideSpeed = 0.0f;

	if (!mLastFrame && keyState[SDL_SCANCODE_SPACE])
	{
		Bullet* bullet = new Bullet(mOwner->GetGame());
		bullet->SetPosition(mOwner->GetGame()->GetPlayer()->GetPosition());
	}

	if (keyState[SDL_SCANCODE_W])
	{
		forwardSpeed += SHIP_SPEED;
	}
	if (keyState[SDL_SCANCODE_S])
	{
		forwardSpeed -= SHIP_SPEED;
	}

	if (keyState[SDL_SCANCODE_A])
	{
		sideSpeed -= SHIP_SPEED;
	}
	if (keyState[SDL_SCANCODE_D])
	{
		sideSpeed += SHIP_SPEED;
	}

	Vector3 velocity(400.0f, sideSpeed, forwardSpeed);
	mVelocity = velocity;

	mLastFrame = keyState[SDL_SCANCODE_SPACE];
}