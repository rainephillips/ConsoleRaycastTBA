#include "Texture.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// TEXTURE NO ALPHA 

Texture::Texture()
	: m_size{ Vector2i( 2, 2 ) }, m_textureData{ nullptr }
{
	m_textureData = GetNewErrorTexture();
}

Texture::Texture(ColorA* image, Vector2i size)
	: m_textureData{ image }, m_size{ size }
{
}

Texture::Texture(ColorA* image, int sizeX, int sizeY)
	: m_textureData{ image }, m_size{ Vector2i( sizeX, sizeY ) }
{
}

Texture::Texture(const char* filepath)
	: m_size{ Vector2i(2, 2) }, m_textureData{ nullptr }
{
	m_textureData = GetNewErrorTexture(); // Just in case file fails to load
	SetTexture(filepath);
}

Texture::~Texture()
{
	if (m_textureData != nullptr)
	{
		delete[] m_textureData;
	}
}

void Texture::SetTexture(ColorA* image, Vector2i size)
{
	// If texture data exists
	if (m_textureData != nullptr)
	{
		delete[] m_textureData;

		m_textureData = image;
		m_size = size;
	}
	else
	{
		// Change texture error texture
		m_size = Vector2i(2, 2);
		m_textureData = GetNewErrorTexture();
	}
}

void Texture::SetTexture(const char* filepath)
{
	if (m_textureData != nullptr)
	{
		// Delete old texture data
		delete[] m_textureData;

		// New map size
		Vector2i size;

		// Amount of color channels the image has
		int channelAmt;

		// Imports image as a char array where it goes 'RGBA, RGBA,RGBA...'
		unsigned char* imgData = stbi_load(filepath, &size.x, &size.y, &channelAmt, 0);

		// if image failed to load for size 0 or lower
		if (imgData == nullptr || size.x < 1 && size.y < 1)
		{
			// Change texture error texture
			m_size = Vector2i(2, 2);
			m_textureData = GetNewErrorTexture();
		}
		else
		{

			// Create new data for texture
			m_size = size;
			m_textureData = new ColorA[size.x * size.y];

			// For each color channel in each x pos and y pos
			for (int x = 0; x < m_size.x; x++)
			{
				for (int y = 0; y < m_size.y; y++)
				{
					// Create default color in case image doesnt have specific channels
					ColorA color = ColorA(0, 0, 0, 255);
					for (int c = 0; c < channelAmt; c++)
					{
						switch (c)
						{
							case 0:
							{
								// Set r value
								color.r = imgData[(y * size.x + x) * channelAmt + c];
								break;
							}
							case 1:
							{
								// Set g value
								color.g = imgData[(y * size.x + x) * channelAmt + c];
								break;
							}
							case 2:
							{
								// Set b value
								color.b = imgData[(y * size.x + x) * channelAmt + c];
								break;
							}
							case 3:
							{
								// Set alpha value
								color.a = imgData[(y * size.x + x) * channelAmt + c];
								break;
							}
							default:
							{
								break;
							}
						}

						
					}

					// Set color of texture at position
					SetTextureColor(x, y, color);
				}
			}
		}
		// Free image
		stbi_image_free(imgData);
	}
	else
	{
		// Change texture error texture
		m_size = Vector2i(2, 2);
		m_textureData = GetNewErrorTexture();
	}
	
}

void Texture::CreateNewTexture(Vector2i size)
{
	// If texture data exists
	if (m_textureData != nullptr)
	{
		// Delete texture
		delete[] m_textureData;

		// Create new texture with right size and garbage data
		m_textureData = new ColorA[size.x * size.y];
		m_size = size;
	}
	else
	{
		// Change texture error texture
		m_size = Vector2i(2, 2);
		m_textureData = GetNewErrorTexture();
	}
}

void Texture::SetTextureColor(int x, int y, ColorA color)
{
	// Check if cords are not out of texture size
	if (x < m_size.x && y < m_size.y)
	{
		m_textureData[y * m_size.x + x] = color;
	}
}

ColorA* Texture::GetNewErrorTexture()
{
	// Create Magenta and Black checker pattern 2x2
	ColorA* textureData = new ColorA[4]
	{
	ColorA(255,0,255,255),
	ColorA(0,0,0,255),
	ColorA(0,0,0,255),
	ColorA(255,0,255,255)
	};

	return textureData;
}

ColorA* Texture::GetTexture()
{
	return m_textureData;
}

ColorA Texture::GetColorFromLocation(int x, int y)
{
	// If in boundaries
	if (x < m_size.x && x >= 0 && y < m_size.y && y >= 0)
	{
		return m_textureData[y * m_size.x + x];
	}

	// Return error color
	return ColorA(255, 0, 255, 255);
}

Vector2i Texture::GetSize()
{
	return m_size;
}