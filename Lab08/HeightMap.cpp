#include "HeightMap.h"
#include <fstream>
#include "CSVHelper.h"
#include "Math.h"

HeightMap::HeightMap()
{

	std::ifstream levelFile;
	levelFile.open("Assets/HeightMap/HeightMap.csv");

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

			mMap.push_back(rowActual);
			rowNum++;
		}
	}
}

bool HeightMap::IsCellOnTrack(int row, int col)
{
	bool onTrack = false;
	bool validC = false;
	if (row < mMap.size())
	{
		for (int i = 0; i < mMap.size(); i++)
		{
			if (col < mMap[i].size())
			{
				validC = true;
				break;
			}
		}

		if (validC)
		{
			if (mMap[row][col] != -1)
			{
				onTrack = true;
			}
		}
	}

	return onTrack;
}

float HeightMap::GetHeightFromCell(int row, int col)
{
	return (-40.0f + mMap[row][col] * 5.0f);
}

Vector3 HeightMap::CellToWorld(int row, int col)
{

	if (!IsCellOnTrack(row, col))
	{
		return Vector3::Zero;
	}

	Vector3 coordinates = Vector3::Zero;
	coordinates.x = GRID_TOP - CELL_SIZE * row;
	coordinates.y = GRID_LEFT + CELL_SIZE * col;
	coordinates.z = GetHeightFromCell(row, col);

	return coordinates;
}

Vector2 HeightMap::WorldToCell(const Vector2& pos)
{
	Vector2 coordinates = Vector2::Zero;
	coordinates.x = (Math::Abs(pos.x - GRID_TOP) + CELL_SIZE / 2.0f) / CELL_SIZE;
	coordinates.y = (Math::Abs(pos.y - GRID_LEFT) + CELL_SIZE / 2.0f) / CELL_SIZE;

	return coordinates;
}

bool HeightMap::IsOnTrack(const Vector2& pos)
{
	Vector2 cell = WorldToCell(pos);
	return IsCellOnTrack(cell.x, cell.y);
}

float HeightMap::GetHeight(const Vector2& pos)
{
	if (IsOnTrack(pos))
	{
		Vector2 cell = WorldToCell(pos);
		return GetHeightFromCell(cell.x, cell.y);
	}

	return -1000.0f;
}