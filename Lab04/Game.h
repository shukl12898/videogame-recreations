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
	void AddBlock(class Block* block);
	void RemoveBlock(class Block* block);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	SDL_Texture* GetTexture(std::string filename);
	Actor* GetGoal() { return mGoal; };
	static const int WIDTH = 600;
	static const int HEIGHT = 448;
	static const int BACKGROUND_X_POS = 3392;
	static const int BACKGROUND_Y_POS = 224;
	static const int COLUMN_SIZE = 32;
	static const int ROW_SIZE = 32;

private:
	Uint32 mPreviousMS = 0;
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	bool mGameisActive = false;
	std::vector<class Actor*> mActors;
	std::vector<class Block*> mBlocks;
	std::vector<class SpriteComponent*> mSprites;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	class Actor* mGoal;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
};