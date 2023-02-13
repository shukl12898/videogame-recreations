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
#include "Spawner.h"
#include "Block.h"
#include "Goomba.h"
#include "Random.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
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
		SDL_CreateWindow("Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
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
	Actor* background = new Actor(this);
	Vector2 screenCenter(BACKGROUND_X_POS, BACKGROUND_Y_POS);
	background->SetPosition(screenCenter);
	SpriteComponent* backgroundSC = new SpriteComponent(background);
	backgroundSC->SetTexture(GetTexture("Assets/Background.png"));

	Mix_Chunk* backgroundMusic = GetSound("Assets/Sounds/Music.ogg");
	mBackgroundMusic = Mix_PlayChannel(-1, backgroundMusic, -1);

	char val = ' ';
	int rowPos = 0;
	std::ifstream levelFile;
	std::string allColumns;

	Vector2 initialPosition(16, 16);
	Vector2 position(16, 16);

	levelFile.open("Assets/Level1.txt");
	while (std::getline(levelFile, allColumns))
	{

		for (int colPos = 0; colPos < allColumns.size(); colPos++)
		{
			Vector2 newPosition(colPos * COLUMN_SIZE, rowPos * ROW_SIZE);
			position = newPosition + initialPosition;
			val = allColumns[colPos];
			if (val == 'A' || val == 'B' || val == 'C' || val == 'D' || val == 'E' || val == 'F' ||
				val == 'G' || val == 'H' || val == 'I')
			{
				Block* block = new Block(this, val);
				block->SetPosition(position);
			}
			else if (val == 'P')
			{
				mPlayer = new Player(this);
				mPlayer->SetPosition(position);
			}
			else if (val == 'Y')
			{
				Spawner* spawner = new Spawner(this);
				spawner->SetPosition(position);
			}
		}

		rowPos++;
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

	for (auto it : mSounds)
	{
		Mix_FreeChunk(it.second);
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

Mix_Chunk* Game::GetSound(const std::string& filename)
{
	if (mSounds.find(filename) == mSounds.end())
	{
		Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
		if (sound == nullptr)
		{
			SDL_Log("Tried to load %s but failed.", filename.c_str());
		}
		else
		{
			sound = Mix_LoadWAV(filename.c_str());
			mSounds[filename] = sound;
		}
	}

	return mSounds[filename];
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

void Game::AddBlock(Block* block)
{
	mBlocks.push_back(block);
}

void Game::RemoveBlock(Block* block)
{
	auto toDelete = std::find(mBlocks.begin(), mBlocks.end(), block);
	mBlocks.erase(toDelete);
}

void Game::AddGoomba(Goomba* goomba)
{
	mGoombas.push_back(goomba);
}

void Game::RemoveGoomba(Goomba* goomba)
{
	auto toDelete = std::find(mGoombas.begin(), mGoombas.end(), goomba);
	mGoombas.erase(toDelete);
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