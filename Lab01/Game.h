#pragma once

#include "SDL2/SDL.h"

class Game
{

public:
	bool Initialize();
	void Shutdown();
	void RunLoop();

private:
	int WIDTH = 1024;
	int HEIGHT = 768;
	int PADDLE_HEIGHT = 100;
	int PADDLE_WIDTH = 10;
	int BALL_SIZE = 10;
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	SDL_Point mPaddle = {5, HEIGHT / 2};
	SDL_Point mBall = {WIDTH / 2, HEIGHT / 2};
	bool mActiveGame = false;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
};