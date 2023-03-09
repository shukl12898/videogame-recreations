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
#include "Random.h"
#include "Player.h"
#include "SideBlock.h"
#include "Block.h"
#include <string>

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
	bool result = mRenderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!result)
	{
		SDL_Log("Failed to initialize");
		return false;
	}

	mAudio = new AudioSystem();

	LoadData();

	mTicksCount = SDL_GetTicks();

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

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}

	mAudio->ProcessInput(state);
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();

	mAudio->Update(deltaTime);

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
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	mPlayer = new Player(this);
	Matrix4 projection =
		Matrix4::CreatePerspectiveFOV(1.22f, WINDOW_WIDTH, WINDOW_HEIGHT, 10.0f, 10000.0f);
	mRenderer->SetProjectionMatrix(projection);

	Vector3 eye(-300, 0, 0);
	Vector3 target(20, 0, 0);
	Vector3 up = Vector3::UnitZ;
	Matrix4 view = Matrix4::CreateLookAt(eye, target, up);
	mRenderer->SetViewMatrix(view);
	mAudio->PlaySound("Music.ogg", true);
	mShipHandle = mAudio->PlaySound("ShipLoop.ogg", true);
}

void Game::LoadBlocks(std::string fileName, float x)
{
	char val = ' ';
	int rowPos = 0;
	std::ifstream levelFile;
	std::string allColumns;

	Vector3 initialPosition(x, -237.5f, 237.5f);
	Vector3 position(x, -237.5f, 237.5f);

	levelFile.open(fileName);
	while (std::getline(levelFile, allColumns))
	{

		for (int colPos = 0; colPos < allColumns.size(); colPos++)
		{
			Vector3 newPosition(0.0f, colPos * COLUMN_SIZE, rowPos * ROW_SIZE);
			position = newPosition + initialPosition;
			val = allColumns[colPos];
			if (val == 'A')
			{
				Block* block = new Block(this, 3);
				block->SetPosition(position);
			}
			else if (val == 'B')
			{
				Block* block = new Block(this, 4);
				block->SetPosition(position);
			}
		}

		rowPos++;
	}

	levelFile.close();
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
