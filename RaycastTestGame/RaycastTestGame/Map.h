#pragma once

#include "Vector2.h"
#include <vector>

enum MapDataType
{
	WALL,
	ROOF,
	FLOOR,
	DATA,
	MAX,
};
using std::vector;

class Sprite;

class Map
{
public:
	Map(int x, int y);
	Map(Vector2i size);
	~Map();

	Vector2i GetMapSize();

	void SetContents(uint64_t* mapData, Vector2i size);
	void SetContentDataType(uint16_t* data, MapDataType dataType ,Vector2i size);
	uint16_t* GetDataTypeBuffer(MapDataType dataType);

	void ClearMapData();

	uint64_t* GetMapData();


private:
	Vector2i m_size;
	uint64_t* m_mapData;

	vector<Sprite*> m_staticSpriteData;
};

