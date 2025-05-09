#include "MapData.h"

MapData::MapData(Vector2i const& size, vector<uint64_t> const& data, Vector2 const& startPos, Vector2 const& startDir, Vector2i const& pos, string const& desc, vector<SpriteData> const& spriteData, vector<size_t> const& wallTex, vector<size_t> const& floorTex, vector<size_t> const& roofTex, Item* const& item)
	: mapSize{ size }, mapData{ data }, startingPostion{ startPos }, startingDirection{ startDir }, mapPosition{ pos }, description{ desc }, sprites{ spriteData }, wallTextures{ wallTex }, floorTextures{ floorTex }, roofTextures{ roofTex }, item{ item }
{
}
