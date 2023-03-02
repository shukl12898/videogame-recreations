#include "TiledBGComponent.h"
#include "Actor.h"
#include "CSVHelper.h"
#include "Game.h"
#include <fstream>
#include <vector>
#include <algorithm>

TiledBGComponent::TiledBGComponent(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{
}

void TiledBGComponent::Draw(SDL_Renderer* renderer)
{

	//draw image PER SQUARE IN THE GRID
	for (int i = 0; i < mCSVTiles.size(); i++)
	{
		for (int j = 0; j < mCSVTiles[0].size(); j++)
		{
			//draws single image
			if (mCSVTiles[i][j] != -1)
			{
				SDL_Rect r;
				r.w = 32;
				r.h = 32;

				// Center the rectangle around the position of the owner
				r.x = j * 32;
				r.y = i * 32;
				r.x -= static_cast<int>(mOwner->GetGame()->GetCameraPos().x);
				r.y -= static_cast<int>(mOwner->GetGame()->GetCameraPos().y);

				SDL_Rect srcRect;
				int tileNumber = mCSVTiles[i][j];
				int numTilesRow = GetTexWidth() / 32;

				srcRect.w = 32;
				srcRect.h = 32;

				srcRect.x = tileNumber % numTilesRow * 32;
				srcRect.y = tileNumber / numTilesRow * 32;

				// Draw (have to convert angle from radians to degrees, and clockwise to counter)
				SDL_RenderCopyEx(renderer, mTexture, &srcRect, &r, 0.0, nullptr, SDL_FLIP_NONE);
				// SDL_Log("%d", tileNumber);
				// break;
			}
		}
		// break;
	}

	//for each square in grid, calculate both correct destination rectange and correct source rectange
}

void TiledBGComponent::LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight)
{
	//open the csv file specified by filename and populate
	std::ifstream levelFile;
	levelFile.open(fileName);
	std::string line;
	int rowNum = 0;

	while (std::getline(levelFile, line))
	{
		std::vector<std::string> rowString;
		std::vector<int> rowActual;

		if (!line.empty())
		{
			rowString = (CSVHelper::Split(line));

			for (int i = 0; i < rowString.size(); i++)
			{
				rowActual.push_back(stoi(rowString[i]));
			}

			mCSVTiles.push_back(rowActual);
			rowNum++;
		}
	}

	// for (int i = 0; i < mCSVTiles.size(); i++)
	// {
	// 	for (int j = 0; j < mCSVTiles[0].size(); j++)
	// 	{
	// 		SDL_Log("%d ", mCSVTiles[i][j]);
	// 	}
	// 	SDL_Log("\n");
	// }
}
