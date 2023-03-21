#include "PlayerMove.h"
#include "VehicleMove.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Random.h"
#include "CollisionComponent.h"
#include "Player.h"
#include "HeightMap.h"

PlayerMove::PlayerMove(Actor* owner)
: VehicleMove(owner)
{
	mOwner = owner;
	mOwner->SetPosition(mOwner->GetGame()->GetHeightMap()->CellToWorld(39, 58));
}

void PlayerMove::Update(float deltaTime)
{

	VehicleMove::Update(deltaTime);

}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	//w OR up arrow key sets pedal to true (else pedal is false)
	if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP])
	{
		SetPedal(true);
	}
	else
	{
		SetPedal(false);
	}

	//a OR left should turn enum to left
	if ((keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) &&
		!(keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]))
	{
		SetTurn(Turn::Left);
	}
	//ELSE if d or right should turn enum to right
	else if ((keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) && !(
				 keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]))
	{
		SetTurn(Turn::Right);
	}
	//if none pressed (or both) then turn enum to none
	else
	{
		SetTurn(Turn::None);
	}
}