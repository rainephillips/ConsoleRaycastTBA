#include "Map.h"

#include "Vector2.h"

Map::Map(int x, int y)
	: m_size{ x, y }
{
}


Map::Map(Vector2i size)
	: m_size{ size }
{
}

Vector2i Map::GetMapSize()
{
	return m_size;
}

void Map::SetContents(int map[24][24], Vector2i size)
{
	for (int column = 0; column < size.y; column++)
	{
		for (int row = 0; row < size.x; row++)
		{
			contents[column][row] = map[column][row];
		}
	}
}

void Map::SetContents(int map[24][24], int x_size, int y_size)
{
	SetContents(map, Vector2i(x_size, y_size));
}
