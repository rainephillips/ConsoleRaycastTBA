#include "Sprite.h"

Sprite::Sprite(Vector2 position, Texture* texture)
	: m_position{ position }, m_texture{ texture }
{
}

Vector2 Sprite::GetPosition()
{
	return m_position;
}

Texture* Sprite::GetTexture()
{
	return m_texture;
}
