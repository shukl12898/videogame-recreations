#include "Player.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "MeshComponent.h"
#include "PlayerMove.h"
#include "Random.h"
#include "PlayerMesh.h"
#include "CameraComponent.h"
#include "HealthComponent.h"
#include "HUD.h"

Player::Player(Game* game)
: Actor(game)
{
	mGame = game;
	mPlayerMove = new PlayerMove(this);
	mCameraComponent = new CameraComponent(this);
	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(50, 100, 50);
	mHUD = new HUD(this);
	mDeathSounds.emplace_back("Glados-PlayerDead1.ogg");
	mDeathSounds.emplace_back("Glados-PlayerDead2.ogg");
	mDeathSounds.emplace_back("Glados-PlayerDead3.ogg");
	mDeathSounds.emplace_back("Glados-PlayerDead4.ogg");
	mDeathSubtitles.emplace_back("Congratulations! The test is now over.");
	mDeathSubtitles.emplace_back(
		"Thank you for participating in this Aperture Science computer-aided enrichment activity.");
	mDeathSubtitles.emplace_back("Goodbye.");
	mDeathSubtitles.emplace_back("You're not a good person. You know that, right?");
	mHealthComponent = new HealthComponent(this);
	mHealthComponent->SetOnDeath([this] {
		int index = Random::GetIntRange(0, 3);
		mDeadSound = mGame->GetAudio()->PlaySound(mDeathSounds[index]);
		mHUD->ShowSubtitle(mDeathSubtitles[index]);
	});
	mHealthComponent->SetOnDamage([this](const Vector3& position) {
		Vector3 playerToLocation = position - GetWorldPosition();
		playerToLocation.z = 0;
		playerToLocation.Normalize();

		Vector3 forward = GetForward();
		forward.z = 0;
		forward.Normalize();

		float dotProduct = Vector3::Dot(forward, playerToLocation);
		float theta = acos(dotProduct);

		Vector3 cross = Vector3::Cross(playerToLocation, forward);
		if (cross.z < 0)
		{
			theta = -1 * theta;
		}
		mHUD->PlayerTakesDamage(theta);
	});
}

void Player::GiveGun()
{
	mHasGun = true;
	new PlayerMesh(mGame);
}

void Player::OnUpdate(float deltaTime)
{
}