#pragma once

#include <vector>

#include "Vector2.h"

class Sprite;
class Texture;

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
	void SetContents(vector<uint64_t> mapData, Vector2i size);

	void SetContentsFromLocation(int x, int y, uint16_t value, MapDataType layer);

	void SetContentDataType(uint16_t* data, MapDataType dataType ,Vector2i size);

	void ClearMapData();
	void ClearSpriteData();

	void AddSprite(Sprite* sprite);

	void SetLayerTexture(size_t texturePos, unsigned short position, MapDataType layer);
	void EmplaceLayerTexture(size_t texturePos, MapDataType layer);

	Texture* GetTexture(unsigned short texture, MapDataType layer, vector<Texture*> textureList);

	Vector2i& GetMapSize();

	vector<Sprite*>& GetSpriteData();
	unsigned int GetSpriteAmt() const;

	uint64_t* GetMapData();
	uint64_t GetDataFromMapPosition(int x, int y) const;
	uint16_t* GetDataTypeBuffer(MapDataType dataType);

private:
	Vector2i m_size;
	uint64_t* m_mapData;

	vector<Sprite*> m_staticSpriteData;

	vector<vector<size_t>*> m_mapTextures;
};

