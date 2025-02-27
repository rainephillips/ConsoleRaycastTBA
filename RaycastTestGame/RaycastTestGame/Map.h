#pragma once

#include "Vector2.h"
#include <vector>

class Sprite;

class Map
{
public:
	Map(unsigned short x, unsigned short y);
	Map(Vector2i size);
	~Map();

	Vector2i GetMapSize();

	void SetContents(unsigned short** map, Vector2i size);
	void SetContents(unsigned short** map, int x_size, int y_size);

	unsigned short* GetWallData();
	unsigned short* GetFloorData();
	unsigned short* GetRoofData();


private:
	Vector2i m_size;
	unsigned short* m_wallData;
	unsigned short* m_floorData;
	unsigned short* m_roofData;

};

