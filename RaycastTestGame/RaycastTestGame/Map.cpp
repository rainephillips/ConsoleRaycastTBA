#include "Map.h"

#include "Vector2.h"
#include "Sprite.h"

Map::Map(int sizeX, int sizeY)
	: m_size{ sizeX, sizeY }
{
	m_mapData = new uint64_t[sizeX * sizeY];
	ClearMapData();
}


Map::Map(Vector2i size)
	: m_size{ size }
{
	m_mapData = new uint64_t[size.x * size.y];
	ClearMapData();
}

Map::~Map()
{
	delete m_mapData;

	for (Sprite* sprite : m_staticSpriteData)
	{
		delete sprite;
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
	if (m_mapData != nullptr)
	{
		if (m_size != size)
		{
			delete m_mapData;

			m_mapData = new uint64_t[size.x * size.y];

			m_size = size;
		}

		for (int column = 0; column < size.y; column++)
		{
			for (int row = 0; row < size.x; row++)
			{
				m_mapData[column * size.x + row] = mapData[column * size.x + row];
			}
		}
	}
}

void Map::SetContentDataType(uint16_t* data, MapDataType dataType, Vector2i size)
{
	if (m_mapData != nullptr)
	{
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
				//m_mapData[column * size.x + row] = 18446462598732840960;
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

		dataArray[cell] = (seperatedData)[dataType];

		GetMapSize();
	}

	return dataArray;
}

void Map::ClearMapData()
{
	if (m_mapData != nullptr)
	{
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
	for (Sprite* sprite : m_staticSpriteData)
	{
		if (sprite != nullptr)
		{
			delete sprite;
		}
	}
	m_staticSpriteData.clear();
}

void Map::AddSprite(Sprite* sprite)
{
	m_staticSpriteData.emplace_back(sprite);
}

uint64_t* Map::GetMapData()
{
	return m_mapData;
}
