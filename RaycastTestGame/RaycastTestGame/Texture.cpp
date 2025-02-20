#include "Texture.h"

Texture::Texture()
	: m_size{ Vector2i( 2, 2 ) }
{
	m_textureData = GetNewErrorTexture();
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
	if (m_textureData != nullptr)
	{
		delete[] m_textureData;
	}
}

void Texture::SetTexture(Color* image, Vector2i size)
{
	if (m_textureData != nullptr)
	{
		delete[] m_textureData;
		m_textureData = image;
		m_size = size;
	}
	else
	{
		m_size = Vector2i(2, 2);
		m_textureData = GetNewErrorTexture();
	}
}

void Texture::CreateNewTexture(Vector2i size)
{
	if (m_textureData != nullptr)
	{
		delete[] m_textureData;
		m_textureData = new Color[size.x * size.y];
		m_size = size;
	}
	else
	{
		m_size = Vector2i(2, 2);
		m_textureData = GetNewErrorTexture();
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

Color* Texture::GetNewErrorTexture()
{
	Color* textureData = new Color[4]
	{ 
	Color(255,0,255), 
	Color(0,0,0),
	Color(255,0,255),
	Color(0,0,0) 
	};

	return textureData;
}

Color* Texture::GetTexture()
{
	return m_textureData;
}

Vector2i Texture::GetSize()
{
	return m_size;
}
