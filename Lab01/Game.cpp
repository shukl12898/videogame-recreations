//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

bool Game::Initialize()
{
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
	mWindow =
		SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	mRenderer =
		SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if ((mRenderer == nullptr) || (mWindow == nullptr))
	{
		mGameisActive = false;
	}

	mGameisActive = true;
	return mGameisActive;
}

void Game::RunLoop()
{
	while (mGameisActive)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mGameisActive = false;
		}
	}

	const Uint8* keyboard = SDL_GetKeyboardState(NULL);

	if (keyboard[SDL_SCANCODE_ESCAPE])
	{
		mGameisActive = false;
	}
	if (keyboard[SDL_SCANCODE_S])
	{
		movement = -1;
	}
	if (keyboard[SDL_SCANCODE_W])
	{
		movement = 1;
	}
}

void Game::UpdateGame()
{
	while ((SDL_GetTicks() - mPreviousMS) < 16)
	{
		//loop until 16ms have elapsed
	}

	int deltaTimeMS = SDL_GetTicks() - mPreviousMS;
	float deltaTimeS = static_cast<float>(deltaTimeMS) / 1000;

	mPreviousMS = SDL_GetTicks();

	if (deltaTimeS > 0.033)
	{
		deltaTimeS = 0.033;
	}

	if (movement < 0)
	{
		if (mPaddle.y + (300 * deltaTimeS) + (PADDLE_HEIGHT / 2) < (HEIGHT - WALL_THICKNESS))
		{
			mPaddle.y = mPaddle.y + (300 * deltaTimeS);
		}
		movement = 0;
	}

	if (movement > 0)
	{
		if ((mPaddle.y - (300 * deltaTimeS) - (PADDLE_HEIGHT / 2)) > WALL_THICKNESS)
		{
			mPaddle.y = mPaddle.y - (300 * deltaTimeS);
		}
		movement = 0;
	}

	mBall.x = mBall.x + (mBallVelocity.x * deltaTimeS);
	mBall.y = mBall.y + (mBallVelocity.y * deltaTimeS);

	if (mBall.y <= WALL_THICKNESS)
	{
		mBallVelocity.y = -1 * mBallVelocity.y;
		mBall.y = WALL_THICKNESS;
	}
	else if (mBall.y >= HEIGHT - WALL_THICKNESS)
	{
		mBallVelocity.y = -1 * mBallVelocity.y;
		mBall.y = HEIGHT - WALL_THICKNESS;
	}
	else if (mBall.x >= WIDTH - WALL_THICKNESS)
	{
		mBallVelocity.x = -1 * mBallVelocity.x;
		mBall.x = WIDTH - WALL_THICKNESS;
	}
	else if (mBall.x <= PADDLE_WIDTH)
	{
		if ((mBall.y <= mPaddle.y + PADDLE_HEIGHT / 2) &&
			(mBall.y >= mPaddle.y - PADDLE_HEIGHT / 2))
		{
			mBall.x = PADDLE_WIDTH;
			mBallVelocity.x = -1 * mBallVelocity.x;
		}
		else
		{
			mGameisActive = false;
		}
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
	SDL_RenderClear(mRenderer);
	SDL_Rect topWall;
	topWall.x = 0;
	topWall.y = 0;
	topWall.w = WIDTH;
	topWall.h = WALL_THICKNESS;
	SDL_Rect rightWall;
	rightWall.x = WIDTH - WALL_THICKNESS;
	rightWall.y = 0;
	rightWall.w = WALL_THICKNESS;
	rightWall.h = HEIGHT;
	SDL_Rect bottomWall;
	bottomWall.x = 0;
	bottomWall.y = HEIGHT - WALL_THICKNESS;
	bottomWall.w = WIDTH;
	bottomWall.h = WALL_THICKNESS;
	SDL_Rect paddle;
	paddle.x = mPaddle.x;
	paddle.y = mPaddle.y - PADDLE_HEIGHT / 2;
	paddle.w = PADDLE_WIDTH;
	paddle.h = PADDLE_HEIGHT;
	SDL_Rect ball;
	ball.x = mBall.x;
	ball.y = mBall.y;
	ball.w = BALL_SIZE;
	ball.h = BALL_SIZE;
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderFillRect(mRenderer, &topWall);
	SDL_RenderFillRect(mRenderer, &rightWall);
	SDL_RenderFillRect(mRenderer, &bottomWall);
	SDL_RenderFillRect(mRenderer, &paddle);
	SDL_RenderFillRect(mRenderer, &ball);
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}