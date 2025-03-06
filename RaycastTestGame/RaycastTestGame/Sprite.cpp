#include "Sprite.h"

Sprite::Sprite(Vector2 position, Texture* texture)
	: m_position{ position }, m_texture{ texture }, 
	m_uvScale{ Vector2(1, 1) }, m_yOffset{ 0.f }
{
}

Sprite::Sprite(Vector2 position, Texture* texture, Vector2 scale, float offset)
	: m_position{ position }, m_texture{ texture },
	m_uvScale{ Vector2(1.f / scale.x, 1.f / scale.y) }, m_yOffset{ offset }
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

Vector2 Sprite::GetScale()
{
	return m_uvScale;
}

float Sprite::GetOffset()
{
	return m_yOffset;
}

void Sprite::SetScale(Vector2 scale)
{
	m_uvScale = Vector2(1.f / scale.x, 1.f / scale.y);
}

void Sprite::SetYOffset(float offset)
{
	m_yOffset = offset;
}
