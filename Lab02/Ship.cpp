#include "Ship.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game)
: Actor(game)
{
	mGame = game;
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));
	mSpriteComponent = sc;
	MoveComponent* mc = new MoveComponent(this);
	mMoveComponent = mc;
	Vector2 shipInitialPosition((game->WIDTH) / 2, (game->HEIGHT) / 2);
	SetPosition(shipInitialPosition);
}

void Ship::OnProcessInput(const Uint8* keyState)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;

	if (keyState[SDL_SCANCODE_S])
	{
		forwardSpeed -= SHIP_FORWARD_SPEED;
	}
	if (keyState[SDL_SCANCODE_W])
	{
		forwardSpeed += SHIP_FORWARD_SPEED;
	}

	if ((keyState[SDL_SCANCODE_W]) ^ (keyState[SDL_SCANCODE_S]))
	{

		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	}
	else
	{
		mSpriteComponent->SetTexture(mGame->GetTexture("Assets/Ship.png"));
	}

	if (keyState[SDL_SCANCODE_A])
	{
		angularSpeed -= SHIP_ANGULAR_SPEED;
	}
	if (keyState[SDL_SCANCODE_D])
	{
		angularSpeed += SHIP_ANGULAR_SPEED;
	}

	mMoveComponent->SetForwardSpeed(forwardSpeed);
	mMoveComponent->SetAngularSpeed(angularSpeed);

	if (keyState[SDL_SCANCODE_SPACE])
	{

		if (GetCooldown() <= 0.0f)
		{
			Laser* laser = new Laser(mGame);
			laser->SetState(ActorState::Active);
			laser->SetPosition(GetPosition());
			laser->SetRotation(GetRotation());
			SetCooldown(1.0f);
		}
	}
}

void Ship::OnUpdate(float deltaTime)
{
	mCooldown -= deltaTime;
}