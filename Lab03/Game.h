#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "SDL2/SDL.h"
#include "Math.h"

class Game
{

public:
	bool Initialize();
	void Shutdown();
	void RunLoop();
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void AddVehicle(class Vehicle* vehicle);
	void RemoveVehicle(class Vehicle* vehicle);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	std::vector<class Vehicle*> GetVehicles() const { return mVehicles; }
	SDL_Texture* GetTexture(std::string filename);
	Vector2 GetFrogPosition();
	static const int WIDTH = 896;
	static const int LEVEL_BLOCK_WIDTH = WIDTH / 14;
	static const int HEIGHT = 1024;
	static const int LEVEL_BLOCK_HEIGHT = HEIGHT / 16;

private:
	Uint32 mPreviousMS = 0;
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	bool mGameisActive = false;
	std::vector<class Actor*> mActors;
	std::vector<class SpriteComponent*> mSprites;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<class Vehicle*> mVehicles;
	class Frog* mFrog;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
};