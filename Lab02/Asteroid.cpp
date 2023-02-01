#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"

Asteroid::Asteroid(Game* game)
: Actor(game)
{
	mSpriteComponent = new SpriteComponent(this);
	mSpriteComponent->SetTexture(game->GetTexture("Assets/Asteroid.png"));
	mMoveComponent = new MoveComponent(this);
	mMoveComponent->SetForwardSpeed(150.0f);
	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));
	Vector2 maxAsteroidPosition(game->WIDTH, game->HEIGHT);
	Vector2 minAsteroidPosition(0, 0);
	SetPosition(Random::GetVector(minAsteroidPosition, maxAsteroidPosition));
	mGame = game;
	mGame->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	mGame->RemoveAsteroid(this);
	for (Component* i : mComponents)
	{
		delete i;
	}
	mComponents.clear();
}

void Asteroid::OnUpdate(float deltaTime)
{
	//Ensures asteroids wrap the screen if they float off the boundaries
	if (GetPosition().x > mGame->WIDTH)
	{
		Vector2 wrapLeft(0.0f, GetPosition().y);
		SetPosition(wrapLeft);
	}
	else if (GetPosition().x < 0)
	{
		Vector2 wrapRight(static_cast<float>(mGame->WIDTH), GetPosition().y);
		SetPosition(wrapRight);
	}

	if (GetPosition().y > mGame->HEIGHT)
	{
		Vector2 wrapTop(GetPosition().x, 0.0f);
		SetPosition(wrapTop);
	}
	else if (GetPosition().y < 0)
	{
		Vector2 wrapBottom(GetPosition().x, static_cast<float>(mGame->HEIGHT));
		SetPosition(wrapBottom);
	}
}