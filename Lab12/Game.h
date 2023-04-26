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

	void ReloadLevel();

	void AddActor(class Actor* actor);
	void AddCollider(class Actor* collider);
	void RemoveActor(class Actor* actor);
	void RemoveCollider(class Actor* collider);
	class Player* GetPlayer() { return mPlayer; };
	void SetPlayer(class Player* player) { mPlayer = player; };
	void LoadBlocks(std::string fileName, float x);
	void AddDoor(class Door* door, std::string doorName) { mDoorNames[doorName] = door; };
	std::unordered_map<std::string, class Door*> GetDoorNames() { return mDoorNames; };

	AudioSystem* GetAudio() { return mAudio; };
	std::vector<class Actor*>& GetColliders() { return mColliders; };
	std::vector<class Actor*>& GetActors() { return mActors; };
	int GetSoundHandle() const { return mSound; };
	class Renderer* GetRenderer() { return mRenderer; }

	void SetBluePortal(class Portal* portal) { mBluePortal = portal; };
	void SetOrangePortal(class Portal* portal) { mOrangePortal = portal; };
	class Portal* GetBluePortal() { return mBluePortal; };
	class Portal* GetOrangePortal() { return mOrangePortal; };

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
	std::vector<class Actor*> mColliders;
	class Player* mPlayer = nullptr;

	class Renderer* mRenderer = nullptr;
	AudioSystem* mAudio = nullptr;

	class Portal* mBluePortal = nullptr;
	class Portal* mOrangePortal = nullptr;

	Uint32 mTicksCount = 0;
	float mStartTimer = 8.5f;
	int mSound = 0;
	bool mIsRunning;
	std::string mNextLevel;

	std::string mCurrentLevel;
	class InputReplay* mInputReplay = nullptr;
	bool mLastFrameF5 = false;

	std::unordered_map<std::string, class Door*> mDoorNames;
};
