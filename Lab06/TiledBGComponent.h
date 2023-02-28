#pragma once
#include "SpriteComponent.h"
#include "SDL2/SDL.h"
#include <vector>

class TiledBGComponent : public SpriteComponent
{
public:
	// (Lower draw order corresponds with further back)
	TiledBGComponent(class Actor* owner, int drawOrder = 50);

	// Draw this sprite
	void Draw(SDL_Renderer* renderer) override;
	void LoadTileCSV(const std::string& filename, int tileWidth, int tileHeight);

protected:
	int mTileWidth;
	int mTileHeight;
	std::vector<std::vector<int>> mCSVTiles;
};
