#include "SpriteData.h"

SpriteData::SpriteData(Vector2 pos, size_t texture)
	: position{ pos }, textureIndex{ texture }, scale{ Vector2{ 1.f, 1.f } }, offset{ 0.f }
{
}

SpriteData::SpriteData(Vector2 pos, size_t texture, Vector2 scale, float offset)
	: position{ pos }, textureIndex{ texture }, scale{ scale }, offset{ offset }
{
}
