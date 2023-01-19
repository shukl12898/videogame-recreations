#pragma once

#include <vector>
#include <unordered_map>
#include "SDL2/SDL.h"

class Game
{

public:
	bool Initialize();
	void Shutdown();
	void RunLoop();
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	SDL_Texture* GetTexture(std::string filename);
	static const int WIDTH = 1024;
	static const int HEIGHT = 768;

private:
	Uint32 mPreviousMS = 0;
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	bool mGameisActive = false;
	std::vector<class Actor*> mActors;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
};