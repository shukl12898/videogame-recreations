//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Log.h"
#include "Frog.h"
#include "Vehicle.h"
#include "Random.h"
#include "SpriteComponent.h"
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include <string>

bool Game::Initialize()
{
	Random::Init();
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
	mWindow = SDL_CreateWindow("Frogger", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH,
							   HEIGHT, 0);
	mRenderer =
		SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	mGameisActive = (mRenderer != nullptr) && (mWindow != nullptr);
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

	std::vector<Actor*> actorsCopy = mActors;

	for (auto i : actorsCopy)
	{
		i->ProcessInput(keyboard);
	}

	if (keyboard[SDL_SCANCODE_ESCAPE])
	{
		mGameisActive = false;
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

	if (deltaTimeS > 0.033f)
	{
		deltaTimeS = 0.033f;
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
	Actor* background = new Actor(this);
	Vector2 screenCenter(WIDTH / 2, HEIGHT / 2);
	background->SetPosition(screenCenter);
	SpriteComponent* backgroundSC = new SpriteComponent(background);
	backgroundSC->SetTexture(GetTexture("Assets/Background.png"));

	char val = ' ';
	int row = 1;
	int col = 1;
	std::ifstream levelFile;

	levelFile.open("Assets/Level.txt");
	while (levelFile >> val)
	{
		if (col == 13)
		{
			col = 0;
			row++;
		}
		if (val == 'X')
		{
			Log* log = new Log(this);
			Vector2 position(row * 64, col * 64);
			log->SetPosition(position);
			log->SetTexture(val);
		}
		if (val == 'Y')
		{
			Log* log = new Log(this);
			Vector2 position(row * 64, col * 64);
			log->SetPosition(position);
			log->SetTexture(val);
		}
		if (val == 'Z')
		{
			Log* log = new Log(this);
			Vector2 position(row * 64, col * 64);
			log->SetPosition(position);
			log->SetTexture(val);
		}
		col++;
	}

	levelFile.close();
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	for (auto it : mTextures)
	{
		SDL_DestroyTexture(it.second);
	}

	mTextures.clear();
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
	for (auto it : mSprites)
	{
		if (it->IsVisible())
		{
			it->Draw(mRenderer);
		}
	}
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

void Game::AddSprite(SpriteComponent* sprite)
{
	mSprites.push_back(sprite);
	std::sort(mSprites.begin(), mSprites.end(), [](SpriteComponent* a, SpriteComponent* b) {
		return a->GetDrawOrder() < b->GetDrawOrder();
	});
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto toDelete = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(toDelete);
}

SDL_Texture* Game::GetTexture(std::string filename)
{
	if (mTextures.find(filename) == mTextures.end())
	{
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (surface == nullptr)
		{
			SDL_Log("Tried to load %s but failed.", filename.c_str());
		}
		else
		{
			SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
			SDL_FreeSurface(surface);
			mTextures[filename] = texture;
		}
	}

	return mTextures[filename];
}