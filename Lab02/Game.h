#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "SDL2/SDL.h"

class Game
{

public:
	bool Initialize();
	void Shutdown();
	void RunLoop();

	//Setters and getters
	void AddActor(class Actor* actor);
	void AddAsteroid(class Asteroid* asteroid);
	void RemoveAsteroid(class Asteroid* asteroid);
	void RemoveActor(class Actor* actor);
	void AddSprite(class SpriteComponent* sprite);
	class Ship* GetShip() const { return mShip; }
	void RemoveSprite(class SpriteComponent* sprite);
	std::vector<class Asteroid*> GetAsteroids() const { return mAsteroids; };
	SDL_Texture* GetTexture(std::string filename);

	//Constants
	static const int WIDTH = 1024;
	static const int HEIGHT = 768;
	const float BACKGROUND_X_POS = 512.0f;
	const float BACKGROUND_Y_POS = 384.0f;
	static const int BACKGROUND_DRAW_ORDER = 80;

private:
	Uint32 mPreviousMS = 0;
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	bool mGameisActive = false;
	class Ship* mShip = nullptr;

	//Vectors of actors, sprites, asteroids and textures
	std::vector<class Actor*> mActors;
	std::vector<class SpriteComponent*> mSprites;
	std::vector<class Asteroid*> mAsteroids;
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
};