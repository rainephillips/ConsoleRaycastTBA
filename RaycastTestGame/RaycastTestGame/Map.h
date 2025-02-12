#pragma once

#include "Vector2.h"

class Map
{
public:
	Map(int x, int y);
	Map(Vector2i size);

	Vector2i GetMapSize();

	void SetContents(int map[24][24], Vector2i size);
	void SetContents(int map[24][24], int x_size, int y_size);

private:
	Vector2i size;

	int contents[24][24];
};

