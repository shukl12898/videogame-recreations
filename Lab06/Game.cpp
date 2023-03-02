//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include "Soldier.h"
#include "Bush.h"
#include "Collider.h"
#include "PathFinder.h"
#include "Random.h"
#include "SpriteComponent.h"
#include "CSVHelper.h"
#include "CollisionComponent.h"
#include "TiledBGComponent.h"
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include <string>

bool Game::Initialize()
{
	Random::Init();
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
	mWindow =
		SDL_CreateWindow("Zelda", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	mRenderer =
		SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	mGameisActive = (mRenderer != nullptr) && (mWindow != nullptr);
	IMG_Init(IMG_INIT_PNG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

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
	Actor* actor = new Actor(this);
	TiledBGComponent* tiledBGC = new TiledBGComponent(actor);
	tiledBGC->LoadTileCSV("Assets/Map/Tiles.csv", TILE_WIDTH, TILE_HEIGHT);
	tiledBGC->SetTexture(GetTexture("Assets/Map/Tiles.png"));

	LoadDataHelper();
}

void Game::LoadDataHelper()
{
	std::ifstream levelFile;
	levelFile.open("Assets/Map/Objects.csv");

	std::string line;

	std::getline(levelFile, line);

	mPathFinder = new PathFinder(this);

	while (std::getline(levelFile, line))
	{
		std::vector<std::string> lineVec = CSVHelper::Split(line);
		if (lineVec[0] == "Player")
		{
			mPlayer = new Player(this);
			int xPos = std::stoi(lineVec[1]);
			int yPos = std::stoi(lineVec[2]);
			int width = std::stoi(lineVec[3]);
			int height = std::stoi(lineVec[4]);
			Vector2 pos(xPos + width / 2, yPos + height / 2);
			mPlayer->SetPosition(pos);
		}
		if (lineVec[0] == "Collider")
		{
			int xPos = std::stoi(lineVec[1]);
			int yPos = std::stoi(lineVec[2]);
			int width = std::stoi(lineVec[3]);
			int height = std::stoi(lineVec[4]);
			Vector2 pos(xPos + width / 2, yPos + height / 2);
			Collider* collider = new Collider(this, width, height);
			collider->SetPosition(pos);
			mColliders.push_back(collider);
		}
		if (lineVec[0] == "Soldier")
		{
			int xPos = std::stoi(lineVec[1]);
			int yPos = std::stoi(lineVec[2]);
			int width = std::stoi(lineVec[3]);
			int height = std::stoi(lineVec[4]);
			int rowStart = std::stoi(lineVec[5]);
			int colStart = std::stoi(lineVec[6]);
			int rowEnd = std::stoi(lineVec[7]);
			int colEnd = std::stoi(lineVec[8]);
			Vector2 pos(xPos + width / 2, yPos + height / 2);

			PathNode* start = mPathFinder->GetPathNode(rowStart, colStart);
			PathNode* end = mPathFinder->GetPathNode(rowEnd, colEnd);

			Soldier* soldier = new Soldier(this, start, end);
			soldier->SetPosition(pos);
		}
		if (lineVec[0] == "Bush")
		{
			int xPos = std::stoi(lineVec[1]);
			int yPos = std::stoi(lineVec[2]);
			int width = std::stoi(lineVec[3]);
			int height = std::stoi(lineVec[4]);
			Vector2 pos(xPos + width / 2, yPos + height / 2);
			Bush* bush = new Bush(this);
			bush->SetPosition(pos);
		}
	}
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
	Mix_CloseAudio();
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