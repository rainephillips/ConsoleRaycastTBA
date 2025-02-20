#include "Texture.h"

Color errorTexture[] = { Color(255,0,255), Color(0,0,0),
					Color(255,0,255), Color(0,0,0) };

Texture::Texture()
	: m_textureData{ errorTexture }, m_size{ Vector2i( 2, 2 ) }
{
}

Texture::Texture(Color* image, Vector2i size)
	: m_textureData{ image }, m_size{ size }
{
}

Texture::Texture(Color* image, int sizeX, int sizeY)
	: m_textureData{ image }, m_size{ Vector2i( sizeX, sizeY ) }
{
}

Texture::~Texture()
{
	delete[] m_textureData;
}

void Texture::SetTexture(Color* image, Vector2i size)
{
	delete[] m_textureData;

	if (m_textureData != nullptr)
	{
		m_textureData = image;
		m_size = size;
	}
	else
	{
		m_textureData = errorTexture;
		m_size = Vector2i(2, 2);
	}
}

void Texture::CreateNewTexture(Vector2i size)
{
	delete[] m_textureData;

	if (m_textureData != nullptr)
	{
		m_textureData = new Color[size.x * size.y];
		m_size = size;
	}
	else
	{
		m_textureData = errorTexture;
		m_size = Vector2i(2, 2);
	}
}

void Texture::SetTextureColor(int x, int y, Color color)
{
	// Check if cords are not out of texture size
	if (x < m_size.x && y < m_size.y)
	{
		m_textureData[y * m_size.x + x] = color;
	}
}

Color* Texture::GetTexture()
{
	return m_textureData;
}

Vector2i Texture::GetSize()
{
	return m_size;
}
