//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

int main(int argc, char** argv)
{
	Game currentGame = Game();
	bool result = currentGame.Initialize();
	if (result)
	{
		currentGame.RunLoop();
	}

	currentGame.Shutdown();
	return 0;
}
