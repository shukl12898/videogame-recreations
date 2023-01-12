#pragma once

#include "SDL2/SDL.h"

class Game
{

public:
	bool Initialize();
	void Shutdown();
	void RunLoop();

private:
	SDL_Window* mWindow;
};