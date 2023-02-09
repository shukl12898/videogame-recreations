#include "Goomba.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "GoombaMove.h"

Goomba::Goomba(Game* game)
: Actor(game)
{
	mGame = game;
	mASC = new AnimatedSprite(this, 150);

	std::vector<SDL_Texture*> walkAnim{mGame->GetTexture("Assets/Goomba/Walk0.png"),
									   mGame->GetTexture("Assets/Goomba/Walk1.png")};
	mASC->AddAnimation("walk", walkAnim);
	std::vector<SDL_Texture*> dead{mGame->GetTexture("Assets/Goomba/Dead.png")};
	mASC->AddAnimation("dead", dead);
	mASC->SetAnimation("walk");

	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(32, 32);
	mGoombaMove = new GoombaMove(this);

	mGame->AddGoomba(this);
}

Goomba::~Goomba()
{
	mGame->RemoveGoomba(this);
	for (Component* i : mComponents)
	{
		delete i;
	}
	mComponents.clear();
}