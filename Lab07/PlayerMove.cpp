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

void PlayerMove::ShieldUpdate(float deltaTime)
{
	if (mShieldLevel == 1 && !mDamageSound)
	{
		mDamageHandle = mOwner->GetGame()->GetAudio()->PlaySound("DamageAlert.ogg", true);
		mDamageSound = true;
	}

	if (mDamageSound && mShieldLevel > 1)
	{
		mOwner->GetGame()->GetAudio()->StopSound(mDamageHandle);
		mDamageSound = false;
	}

	if (mShieldLevel == 0)
	{
		mOwner->SetState(ActorState::Paused);
		mOwner->GetGame()->GetAudio()->PlaySound("ShipDie.wav", false);
		mOwner->GetGame()->GetAudio()->StopSound(mOwner->GetGame()->GetShipHandle());
		mOwner->GetGame()->GetAudio()->StopSound(mDamageHandle);
	}

	if (mDamage && mDamageTime == 1.0f)
	{
		mShieldLevel--;
		mOwner->GetGame()->GetAudio()->PlaySound("ShipHit.wav", false);
	}
}

void PlayerMove::Update(float deltaTime)
{

	ShieldUpdate(deltaTime);

	if (mSpeedTimer >= 10.0f)
	{
		mSpeedMultiplier += 0.15f;
		mSpeedTimer = 0.0f;
	}

	if (mRoll)
	{
		if (mRollTimer > 0)
		{
			mRollTimer -= deltaTime;
			mOwner->SetRollAngle(mOwner->GetRollAngle() + (8.0f * Math::Pi * deltaTime));
		}
		else
		{
			mRoll = false;
			mRollTimer = 0.5f;
			mOwner->SetRollAngle(0.0f);
		}
	}

	if (mDamage)
	{
		if (mDamageTime > 0)
		{
			mDamageTime -= deltaTime;
		}
		else
		{
			mDamage = false;
			mDamageTime = 1.0f;
		}
	}

	Vector3 position = mOwner->GetPosition() + (mVelocity * mSpeedMultiplier * deltaTime);
	position.y = Math::Clamp<float>(position.y, -180, 180);
	position.z = Math::Clamp<float>(position.z, -225, 225);

	Game* game = mOwner->GetGame();
	int i = 0;
	int j = 0;

	while (mBlocksEnd - mOwner->GetPosition().x < 4000)
	{
		int sideText = 0;
		int topText = 0;

		while (i >= mSidePattern.size())
		{
			i -= mSidePattern.size();
		}

		while (j >= mTopPattern.size())
		{
			j -= mSidePattern.size();
		}

		sideText = mSidePattern[i];
		topText = mTopPattern[j];

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

		mOwner->GetGame()->LoadBlocks(fileName, static_cast<float>(mObstaclesEnd));
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
				mDamage = true;
			}
		}
	}
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{

	float forwardSpeed = 0.0f;
	float sideSpeed = 0.0f;

	if (!mLastFrameQ && keyState[SDL_SCANCODE_Q] && !mRoll)
	{
		if (mShieldLevel < 3)
		{
			mShieldLevel++;
		}
		mOwner->GetGame()->GetAudio()->PlaySound("Boost.wav", false);
		mOwner->SetRollAngle(8.0f * Math::Pi);
		mRoll = true;
	}

	if (!mLastFrameSpace && keyState[SDL_SCANCODE_SPACE])
	{
		Bullet* bullet = new Bullet(mOwner->GetGame());
		bullet->SetPosition(mOwner->GetGame()->GetPlayer()->GetPosition());
		bullet->SetSpeed(bullet->GetSpeed() * mSpeedMultiplier);
		mOwner->GetGame()->GetAudio()->PlaySound("Shoot.wav", false);
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

	mLastFrameSpace = keyState[SDL_SCANCODE_SPACE];
	mLastFrameQ = keyState[SDL_SCANCODE_Q];
}