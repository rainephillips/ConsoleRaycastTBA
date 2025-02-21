#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

void Texture::SetTexture(const char* filepath)
{

	if (m_textureData != nullptr)
	{
		delete[] m_textureData;

		Vector2i size = Vector2i();
		int channelAmt;
		unsigned char* imgData = stbi_load(filepath, &size.x, &size.y, &channelAmt, 3);

		if (imgData == nullptr)
		{
			m_size = Vector2i(2, 2);
			m_textureData = GetNewErrorTexture();
		}
		else
		{

			// Create new data for texture
			m_size = size;
			m_textureData = new Color[size.x * size.y];


			for (int x = 0; x < m_size.x; x++)
			{
				for (int y = 0; y < m_size.y; y++)
				{
					Color color;
					for (int c = 0; c < channelAmt; c++)
					{
						switch (c)
						{
						case 0:
						{
							color.r = imgData[(y * size.x + x) * channelAmt + c];
							break;
						}
						case 1:
						{
							color.g = imgData[(y * size.x + x) * channelAmt + c];
							break;
						}
						case 2:
						{
							color.b = imgData[(y * size.x + x) * channelAmt + c];
							break;
						}
						default:
						{
							break;
						}
						}
					}
					m_textureData[y * m_size.x + x] = color;
				}
			}
		}
		stbi_image_free(imgData);
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
	Color(0,0,0),
	Color(255,0,255)
	};

	return textureData;
}

Color* Texture::GetTexture()
{
	return m_textureData;
}

Color Texture::GetColorFromLocation(int x, int y)
{
	if (x < m_size.x && y < m_size.y)
	{
		return m_textureData[y * m_size.x + x];
	}

	return Color(255, 0, 255);
}

Vector2i Texture::GetSize()
{
	return m_size;
}
