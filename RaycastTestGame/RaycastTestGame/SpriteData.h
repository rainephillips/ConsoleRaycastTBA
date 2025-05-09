#pragma once

#include "Vector2.h"

struct SpriteData
{
public:
	Vector2 position;
	size_t textureIndex;
	Vector2 scale;
	float offset;

public:
	SpriteData(Vector2 pos, size_t texture);
	SpriteData(Vector2 pos, size_t texture, Vector2 scale, float offset);
};

