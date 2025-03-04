#include "Sprite.h"

Sprite::Sprite(Vector2 position, Texture* texture)
	: m_position{ position }, m_texture{ texture }
{
}

Vector2 Sprite::GetPosition()
{
	return Vector2();
}

Texture* Sprite::GetTexture()
{
	return m_texture;
}
