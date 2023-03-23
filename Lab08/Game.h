#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "AudioSystem.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define emscripten_cancel_main_loop()
#endif

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void EmRunIteration()
	{
		if (!mIsRunning)
		{
			emscripten_cancel_main_loop();
		}
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	class Player* GetPlayer() { return mPlayer; };
	class Enemy* GetEnemy() { return mEnemy; };
	class HeightMap* GetHeightMap() { return mHeightMap; };
	void LoadBlocks(std::string fileName, float x);

	AudioSystem* GetAudio() { return mAudio; };
	int GetSoundHandle() { return mSound; };
	class Renderer* GetRenderer() { return mRenderer; }

	const float WINDOW_WIDTH = 1024.0f;
	const float WINDOW_HEIGHT = 768.0f;
	const float COLUMN_SIZE = 25;
	const float ROW_SIZE = -25;

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// All the actors in the game
	std::vector<class Actor*> mActors;
	class Player* mPlayer = nullptr;
	class Enemy* mEnemy = nullptr;
	class Actor* mTrack = nullptr;
	class HeightMap* mHeightMap = nullptr;

	class Renderer* mRenderer = nullptr;
	AudioSystem* mAudio = nullptr;

	Uint32 mTicksCount = 0;
	float mStartTimer = 8.5f;
	int mSound = 0;
	bool mIsRunning;
};
