#include <vector>
#include "Math.h"

class HeightMap
{
public:
	HeightMap();
	Vector3 CellToWorld(int row, int col);
	Vector2 WorldToCell(const Vector2& pos);
	bool IsOnTrack(const Vector2& pos);
	float GetHeight(const Vector2& pos);

	const float CELL_SIZE = 40.55f;
	const float GRID_TOP = 1280.0f;
	const float GRID_LEFT = -1641.0f;

private:
	bool IsCellOnTrack(int row, int col);
	float GetHeightFromCell(int row, int col);

	std::vector<std::vector<int>> mMap;
};