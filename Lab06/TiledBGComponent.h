#pragma once
#include "SpriteComponent.h"
#include "SDL2/SDL.h"
#include <vector>
#include <string>

class TiledBGComponent : public SpriteComponent
{
public:
	// (Lower draw order corresponds with further back)
	TiledBGComponent(class Actor* owner, int drawOrder = 50);

	// Draw this sprite
	void Draw(SDL_Renderer* renderer) override;
	void LoadTileCSV(const std::string& filename, int tileWidth, int tileHeight);
	static const int TILE_DIMENSION = 32;

protected:
	int mTileWidth = 0;
	int mTileHeight = 0;
	std::vector<std::vector<int>> mCSVTiles;
};
