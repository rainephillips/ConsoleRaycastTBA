#pragma once

#include <vector>
#include <string>

#include "SpriteData.h"

using std::string;
using std::vector; 

class Item;

struct MapData
{
public:
	Vector2i mapSize;
	vector<uint64_t> mapData;

	Vector2 startingPostion;
	Vector2 startingDirection;

	Vector2i mapPosition;

	string description;

	vector<SpriteData> sprites;
	
	vector<size_t> wallTextures;
	vector<size_t> floorTextures;
	vector<size_t> roofTextures;

	Item* item;


public:
	MapData
	(
		Vector2i const& size,
		vector<uint64_t> const& data,
		Vector2 const& startPos, 
		Vector2 const& startDir,
		Vector2i const& pos,
		string const& desc,
		vector<SpriteData> const& spriteData,
		vector<size_t> const& wallTex,
		vector<size_t> const& floorTex,
		vector<size_t> const& roofTex,
		Item* const& item
	);
};