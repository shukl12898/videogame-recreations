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
	Game current = Game();
	bool result = current.Initialize();
	if (result)
	{
		current.RunLoop();
	}

	current.Shutdown();
	return 0;
}
