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
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	Vector2& GetCameraPos() { return mCamera; };
	void SetCameraPos(Vector2 pos) { mCamera = pos; };
	SDL_Texture* GetTexture(std::string filename);
	class Actor* GetGoal() { return mGoal; };
	class PathFinder*& GetPathFinder() { return mPathFinder; };
	class Player* GetPlayer() { return mPlayer; };
	class AudioSystem* GetAudioSystem() { return mAudioSystem; };
	const std::vector<class Collider*>& GetColliders() { return mColliders; }
	std::vector<class EnemyComponent*>& GetEnemyComponents() { return mEnemyComponents; }
	int GetBackgroundMusicChannel() const { return mBackgroundMusic; };
	static const int WIDTH = 512;
	static const int HEIGHT = 448;
	static const int TILE_WIDTH = 32;
	static const int TILE_HEIGHT = 32;
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
	class PathFinder* mPathFinder;
	std::vector<class Actor*> mActors;
	std::vector<class SpriteComponent*> mSprites;
	std::vector<class Collider*> mColliders;
	std::vector<class EnemyComponent*> mEnemyComponents;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	class Actor* mGoal = nullptr;
	class Player* mPlayer = nullptr;
	class AudioSystem* mAudioSystem = nullptr;
	int mStartSound;
	bool mBackMusic = false;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void LoadDataHelper();
	void UnloadData();
};