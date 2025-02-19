#include "Texture.h"

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

Color* Texture::GetTexture()
{
	return m_textureData;
}

Vector2i Texture::GetSize()
{
	return m_size;
}
