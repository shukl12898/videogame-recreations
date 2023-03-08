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
	for (size_t i = 0; i < mCSVTiles.size(); i++)
	{
		for (size_t j = 0; j < mCSVTiles[0].size(); j++)
		{
			//draws single image
			if (mCSVTiles[i][j] != -1)
			{
				SDL_Rect r;
				r.w = TILE_DIMENSION;
				r.h = TILE_DIMENSION;

				// Center the rectangle around the position of the owner
				r.x = j * TILE_DIMENSION;
				r.y = i * TILE_DIMENSION;
				r.x -= static_cast<int>(mOwner->GetGame()->GetCameraPos().x);
				r.y -= static_cast<int>(mOwner->GetGame()->GetCameraPos().y);

				SDL_Rect srcRect;
				int tileNumber = mCSVTiles[i][j];
				int numTilesRow = GetTexWidth() / TILE_DIMENSION;

				srcRect.w = TILE_DIMENSION;
				srcRect.h = TILE_DIMENSION;

				srcRect.x = tileNumber % numTilesRow * TILE_DIMENSION;
				srcRect.y = tileNumber / numTilesRow * TILE_DIMENSION;

				// Draw (have to convert angle from radians to degrees, and clockwise to counter)
				SDL_RenderCopyEx(renderer, mTexture, &srcRect, &r, 0.0f, nullptr, SDL_FLIP_NONE);
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
}
