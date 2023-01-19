//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include <algorithm>
#include <SDL2/SDL_image.h>>

bool Game::Initialize()
{
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
	mWindow = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH,
							   HEIGHT, 0);
	mRenderer =
		SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	mGameisActive = !((mRenderer == nullptr) || (mWindow == nullptr));
	IMG_Init(IMG_INIT_PNG);
	LoadData();

	return mGameisActive;
}

void Game::RunLoop()
{
	while (mGameisActive)
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
			mGameisActive = false;
		}
	}

	const Uint8* keyboard = SDL_GetKeyboardState(NULL);

	if (keyboard[SDL_SCANCODE_ESCAPE])
	{
		mGameisActive = false;
	}
	if (keyboard[SDL_SCANCODE_S])
	{
	}
	if (keyboard[SDL_SCANCODE_W])
	{
	}
}

void Game::UpdateGame()
{
	while ((SDL_GetTicks() - mPreviousMS) < 16)
	{
		//loop until 16ms have elapsed
	}

	int deltaTimeMS = SDL_GetTicks() - mPreviousMS;
	float deltaTimeS = static_cast<float>(deltaTimeMS) / 1000;

	mPreviousMS = SDL_GetTicks();

	if (deltaTimeS > 0.033)
	{
		deltaTimeS = 0.033;
	}

	std::vector<Actor*> actorsCopy = mActors;

	for (auto i : actorsCopy)
	{
		i->Update(deltaTimeS);
	}

	std::vector<Actor*> toDestroy;

	for (auto j : mActors)
	{
		if (j->GetState() == ActorState::Destroy)
		{
			toDestroy.push_back(j);
		}
	}

	for (auto k : toDestroy)
	{
		delete k;
	}
}

void Game::LoadData()
{
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.push_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto toDelete = std::find(mActors.begin(), mActors.end(), actor);
	mActors.erase(toDelete);
}

SDL_Texture* Game::GetTexture(std::string filename)
{
	if (mTextures.find(filename) == mTextures.end())
	{
		SDL_Surface* surface = IMG_Load(filename.c_str());
		SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
		SDL_FreeSurface(surface);
		mTextures[filename] = texture;
	}

	return mTextures[filename];
}