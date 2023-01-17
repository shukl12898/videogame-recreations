#pragma once

#include "SDL2/SDL.h"

class Game
{

public:
	bool Initialize();
	void Shutdown();
	void RunLoop();
	static const int WIDTH = 1024;
	static const int HEIGHT = 768;
	static const int PADDLE_HEIGHT = 100;
	static const int PADDLE_WIDTH = 10;
	static const int BALL_SIZE = 10;
	static const int WALL_THICKNESS = 20;

private:
	int movement = 0;
	Uint32 mPreviousMS = 0;
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	SDL_Point mPaddle = {5, HEIGHT / 2};
	SDL_Point mBall = {WIDTH / 2, HEIGHT / 2};
	SDL_Point mBallVelocity = {-200, 200};
	bool mGameisActive = false;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
};