#include "Map.h"

#include "Sprite.h"

Map::Map(int sizeX, int sizeY)
	: m_size{ sizeX, sizeY }
{
	// Create new map data and set all values to 0 to prevent misread info
	m_mapData = new uint64_t[sizeX * sizeY];
	ClearMapData();

	// Add 1 texture layer per layer
	for (size_t i = 0; i < MapDataType::MAX - 1; i++)
	{
		m_mapTextures.emplace_back(new vector<size_t>);
	}
}


Map::Map(Vector2i size)
	: m_size{ size }
{
	// Create new map data and set all values to 0 to prevent misread info
	m_mapData = new uint64_t[size.x * size.y];
	ClearMapData();

	// Add 1 texture layer per layer
	for (size_t i = 0; i < MapDataType::MAX - 1; i++)
	{
		m_mapTextures.emplace_back(new vector<size_t>);
	}
}

Map::~Map()
{
	delete m_mapData;

	// For each sprite if it exists delete it
	for (Sprite* sprite : m_staticSpriteData)
	{
		if (sprite != nullptr)
		{
			delete sprite;
		}
	}

	// delete all texture vectors
	for (int i = 0; i < m_mapTextures.size(); i++)
	{
		delete m_mapTextures[i];
	}
}

Vector2i& Map::GetMapSize()
{
	return m_size;
}

vector<Sprite*>& Map::GetSpriteData()
{
	return m_staticSpriteData;
}

unsigned int Map::GetSpriteAmt()
{
	return m_staticSpriteData.size();
}

void Map::SetContents(uint64_t* mapData, Vector2i size)
{
	// If map data exists
	if (m_mapData != nullptr)
	{
		// If map size is different delete map and make new one with correct size
		if (m_size != size)
		{
			delete m_mapData;

			m_mapData = new uint64_t[size.x * size.y];

			m_size = size;
		}

		// For each cell assign data
		for (int column = 0; column < size.y; column++)
		{
			for (int row = 0; row < size.x; row++)
			{
				m_mapData[column * size.x + row] = mapData[column * size.x + row];
			}
		}
	}
}

void Map::SetContentsFromLocation(int x, int y, uint16_t value, MapDataType layer)
{
	// If not out of bounds
	if (x >= 0 && y >= 0 && x < m_size.x && y < m_size.y)
	{
		// Separate uint64_to 4 shorts (8 bytes 64 bits)
		uint16_t* seperatedData = reinterpret_cast<uint16_t*>(&m_mapData[y * m_size.x + x]);

		// Change Datatype
		seperatedData[layer] = value;

		// Put seperated data back into mapData
		m_mapData[y * m_size.x + x] = *reinterpret_cast<uint64_t*>(seperatedData);
	}
}

void Map::SetContentDataType(uint16_t* data, MapDataType dataType, Vector2i size)
{
	// If map exists
	if (m_mapData != nullptr)
	{
		// If size invalid abort
		if (m_size != size)
		{
			return;
		}

		for (int column = 0; column < size.y; column++)
		{
			for (int row = 0; row < size.x; row++)
			{
				// Separate uint64_to 4 shorts (8 bytes 64 bits)
				uint16_t* seperatedData = reinterpret_cast<uint16_t*>(&m_mapData[column * size.x + row]);

				// Change Datatype
				seperatedData[dataType] = data[column * size.x + row];

				// Put seperated data back into mapData
				m_mapData[column * size.x + row] = *reinterpret_cast<uint64_t*>(seperatedData);

			}
		}
	}
}

uint16_t* Map::GetDataTypeBuffer(MapDataType dataType)
{
	uint16_t* dataArray = new uint16_t[m_size.x * m_size.y];

	for (int cell = 0; cell < (m_size.x * m_size.y); cell++)
	{
		// Separate uint64_to 4 shorts (8 bytes 64 bits)
		uint16_t* seperatedData = reinterpret_cast<uint16_t*>(&m_mapData[cell]);

		// Cast mapData into shorts and get the datatype
		dataArray[cell] = seperatedData[dataType];
	}

	return dataArray;
}

void Map::ClearMapData()
{
	// If map exists
	if (m_mapData != nullptr)
	{
		// Set all cells to 0
		for (int y = 0; y < m_size.y; y++)
		{
			for (int x = 0; x < m_size.x; x++)
			{
				m_mapData[y * m_size.x + x] = 0;
			}
		}
	}
}

void Map::ClearSpriteData()
{
	// If sprite exists delete it
	for (Sprite* sprite : m_staticSpriteData)
	{
		if (sprite != nullptr)
		{
			delete sprite;
		}
	}

	// Clear vector
	m_staticSpriteData.clear();
}

void Map::AddSprite(Sprite* sprite)
{
	m_staticSpriteData.emplace_back(sprite);
}

void Map::SetLayerTexture(size_t texturePos, unsigned short position, MapDataType layer)
{
	// Add location of texture to respective layer
	m_mapTextures[layer]->insert(m_mapTextures[layer]->begin() + position, texturePos);
}

void Map::EmplaceLayerTexture(size_t texturePos, MapDataType layer)
{
	// Emplace back location of texture to respective layer
	m_mapTextures[layer]->emplace_back(texturePos);
}

Texture* Map::GetTexture(unsigned short texture, MapDataType layer, vector<Texture*> textureList)
{
	// Get texture from layer and short value
	if (texture < m_mapTextures[layer]->size()) // Checks if not out of range
	{
		return textureList[ m_mapTextures[layer]->at(texture) ];
	}
	else
	{
		// Return first texture - should be error texture
		return textureList[0];
	}
	
}

uint64_t* Map::GetMapData()
{
	return m_mapData;
}
