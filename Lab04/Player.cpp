#include "Player.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"

Player::Player(Game* game)
: Actor(game)
{
	mGame = game;
	mASC = new AnimatedSprite(this, 200);

	std::vector<SDL_Texture*> idle{mGame->GetTexture("Assets/Mario/Idle.png")};
	mASC->AddAnimation("idle", idle);
	std::vector<SDL_Texture*> dead{mGame->GetTexture("Assets/Mario/Dead.png")};
	mASC->AddAnimation("dead", dead);
	std::vector<SDL_Texture*> jumpleft{mGame->GetTexture("Assets/Mario/JumpLeft.png")};
	mASC->AddAnimation("jumpLeft", jumpleft);
	std::vector<SDL_Texture*> jumpright{mGame->GetTexture("Assets/Mario/JumpRight.png")};
	mASC->AddAnimation("jumpRight", jumpright);
	std::vector<SDL_Texture*> runLeft{mGame->GetTexture("Assets/Mario/RunLeft0.png"),
									  mGame->GetTexture("Assets/Mario/RunLeft1.png"),
									  mGame->GetTexture("Assets/Mario/RunLeft2.png")};
	mASC->AddAnimation("runLeft", runLeft);
	std::vector<SDL_Texture*> runRight{mGame->GetTexture("Assets/Mario/RunRight0.png"),
									   mGame->GetTexture("Assets/Mario/RunRight1.png"),
									   mGame->GetTexture("Assets/Mario/RunRight2.png")};
	mASC->AddAnimation("runRight", runRight);
	mASC->SetAnimation("idle");

	mCollisionComponent = new CollisionComponent(this);
	mCollisionComponent->SetSize(32, 32);
	mPlayerMove = new PlayerMove(this);
}
