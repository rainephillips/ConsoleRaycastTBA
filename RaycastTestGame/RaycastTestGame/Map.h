#pragma once

#include <vector>

#include "Vector2.h"

class Sprite;

using std::vector;

enum MapDataType
{
	WALL,
	ROOF,
	FLOOR,
	INTERACTION,
	MAX,
};

class Map
{
public:
	Map(int x, int y);
	Map(Vector2i size);
	~Map();

public:
	void SetContents(uint64_t* mapData, Vector2i size);
	void SetContentDataType(uint16_t* data, MapDataType dataType ,Vector2i size);

	void ClearMapData();
	void ClearSpriteData();

	void AddSprite(Sprite* sprite);

	Vector2i& GetMapSize();

	vector<Sprite*>& GetSpriteData();

	uint64_t* GetMapData();
	uint16_t* GetDataTypeBuffer(MapDataType dataType);

private:
	Vector2i m_size;
	uint64_t* m_mapData;

	vector<Sprite*> m_staticSpriteData;
};

