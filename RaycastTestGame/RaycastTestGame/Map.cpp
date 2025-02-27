#include "Map.h"

#include "Vector2.h"

Map::Map(unsigned short sizeX, unsigned short sizeY)
	: m_size{ sizeX, sizeY }
{
	m_wallData = new unsigned short[sizeX * sizeY];
	m_floorData = new unsigned short[sizeX * sizeY];
	m_roofData = new unsigned short[sizeX * sizeY];
}


Map::Map(Vector2i size)
	: m_size{ size }
{
	m_wallData = new unsigned short[size.x * size.y];
	m_floorData = new unsigned short[size.x * size.y];
	m_roofData = new unsigned short[size.x * size.y];
}

Map::~Map()
{
	delete m_wallData;
	delete m_floorData;
	delete m_roofData;
}

Vector2i Map::GetMapSize()
{
	return m_size;
}

void Map::SetContents(unsigned short** map, Vector2i size)
{
	if (m_wallData != nullptr)
	{
		if (m_size != size)
		{
			delete m_wallData;
			m_wallData = new unsigned short[size.x * size.y];
			m_size = size;
		}

		for (int column = 0; column < size.y; column++)
		{
			for (int row = 0; row < size.x; row++)
			{
				m_wallData[column * size.x + row] = map[column][row];
			}
		}
	}
}

void Map::SetContents(unsigned short** map, int x_size, int y_size)
{
	SetContents(map, Vector2i(x_size, y_size));
}

unsigned short* Map::GetWallData()
{
	return m_wallData;
}

unsigned short* Map::GetFloorData()
{
	return m_floorData;
}

unsigned short* Map::GetRoofData()
{
	return m_roofData;
}
