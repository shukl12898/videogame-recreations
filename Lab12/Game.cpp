//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "MeshComponent.h"
#include "Random.h"
#include "Player.h"
#include <string>
#include "LevelLoader.h"
#include "InputReplay.h"
#include <SDL2/SDL_ttf.h>

Game::Game()
: mIsRunning(true)
{
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mRenderer = new Renderer(this);
	// On Mac, tell SDL that CTRL+Click should generate a Right Click event
	SDL_SetHint(SDL_HINT_MAC_CTRL_CLICK_EMULATE_RIGHT_CLICK, "1");
	// Enable relative mouse mode
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// Clear any saved values
	SDL_GetRelativeMouseState(nullptr, nullptr);
	bool result = mRenderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!result)
	{
		SDL_Log("Failed to initialize");
		return false;
	}

	mAudio = new AudioSystem();

	TTF_Init();

	LoadData();

	mTicksCount = SDL_GetTicks();

	mInputReplay = new InputReplay(this);

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	if (state[SDL_SCANCODE_P])
	{
		mInputReplay->StartPlayback(mCurrentLevel);
	}

	if (!mLastFrameF5 && state[SDL_SCANCODE_F5])
	{
		ReloadLevel();
	}

	int x = 0;
	int y = 0;
	Uint32 mouseButtons = SDL_GetRelativeMouseState(&x, &y);
	Vector2 relativeMouse(x, y);

	mInputReplay->InputPlayback(state, mouseButtons, relativeMouse);

	std::vector<Actor*> copy = mActors;
	for (Actor* actor : copy)
	{
		actor->ProcessInput(state, mouseButtons, relativeMouse);
	}

	mAudio->ProcessInput(state);
	mLastFrameF5 = state[SDL_SCANCODE_F5];
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();

	mInputReplay->Update(deltaTime);

	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}

	if (!mNextLevel.empty())
	{
		UnloadData();
		mInputReplay->StopPlayback();
		mAudio->StopAllSounds();
		mBluePortal = nullptr;
		mOrangePortal = nullptr;
		mCurrentLevel = mNextLevel;
		LevelLoader::Load(this, mCurrentLevel);
		mNextLevel = "";
	}
}

void Game::ReloadLevel()
{
	mNextLevel = mCurrentLevel;
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	Matrix4 projection =
		Matrix4::CreatePerspectiveFOV(1.22f, WINDOW_WIDTH, WINDOW_HEIGHT, 10.0f, 10000.0f);
	mRenderer->SetProjectionMatrix(projection);
	mAudio->CacheAllSounds();
	mCurrentLevel = "Assets/Level01.json";
	LevelLoader::Load(this, mCurrentLevel);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}
}

void Game::Shutdown()
{
	UnloadData();
	delete mAudio;
	mRenderer->Shutdown();
	delete mRenderer;
	delete mInputReplay;
	TTF_Quit();
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		mActors.erase(iter);
	}
}

void Game::AddCollider(Actor* collider)
{
	mColliders.emplace_back(collider);
}

void Game::RemoveCollider(Actor* collider)
{
	auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
	if (iter != mColliders.end())
	{
		mColliders.erase(iter);
	}
}
