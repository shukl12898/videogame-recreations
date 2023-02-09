#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "SDL2/SDL.h"
#include "Math.h"
#include "SDL2/SDL_mixer.h"

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
	void AddGoomba(class Goomba* goomba);
	void RemoveGoomba(class Goomba* goomba);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	Mix_Chunk* GetSound(const std::string& filename);
	Vector2& GetCameraPos() { return mCamera; };
	SDL_Texture* GetTexture(std::string filename);
	std::vector<class Block*> GetBlocks() { return mBlocks; };
	std::vector<class Goomba*> GetGoombas() { return mGoombas; };
	class Actor* GetGoal() { return mGoal; };
	class Player* GetPlayer() { return mPlayer; };
	int GetBackgroundMusicChannel() const { return mBackgroundMusic; };
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
	int mBackgroundMusic = 0;
	Vector2 mCamera;
	std::vector<class Actor*> mActors;
	std::vector<class Block*> mBlocks;
	std::vector<class Goomba*> mGoombas;
	std::vector<class SpriteComponent*> mSprites;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::unordered_map<std::string, Mix_Chunk*> mSounds;
	class Actor* mGoal = nullptr;
	class Player* mPlayer = nullptr;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
};