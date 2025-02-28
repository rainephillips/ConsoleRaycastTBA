#include "Sprite.h"

Sprite::Sprite(Vector2 position, TextureA* texture)
	: m_position{ position }, m_texture{ texture }
{
}

Vector2 Sprite::GetPosition()
{
	return Vector2();
}

TextureA* Sprite::GetTexture()
{
	return nullptr;
}
