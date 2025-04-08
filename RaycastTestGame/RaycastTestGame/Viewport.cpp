#include "Viewport.h"

#include "Vector2.h"

Viewport::Viewport()
	: position{ 1, 1 }, size{100, 30 }
{
	// Create new ascii and color screen buffer with the size of the viewport
	m_colorScreenBuffer = new Color[size.y * size.x];
}


Viewport::Viewport(Vector2i position, Vector2i size)
	: position{ position }, size { size }
{
	// Create new ascii and color screen buffer with the size of the viewport
	m_colorScreenBuffer = new Color[size.y * size.x];
}

Viewport::~Viewport()
{
	if (m_colorScreenBuffer != nullptr)
	{
		delete[] m_colorScreenBuffer;
	}
}

void Viewport::AddColorToBuffer(int x, int y, Color color)
{
	m_colorScreenBuffer[y * size.x + x] = color;
}

void Viewport::AddColorAToBuffer(int x, int y, ColorA color)
{
	// Create color variable as the result of layering the viewport color with the alpha color
	Color resultColor = m_colorScreenBuffer[y * size.x + x];
	m_colorScreenBuffer[y * size.x + x] = color.LayerRGBAOnRGB(resultColor);
}

void Viewport::SetColorBuffer(Vector2i size, Color* buffer)
{
	// If color buffer exists
	if (m_colorScreenBuffer != nullptr)
	{
		// Delete buffer
		delete[] m_colorScreenBuffer;

		// Assign new size and buffer
		this->size = size;
		m_colorScreenBuffer = new Color[size.x * size.y];

		// For each pixel in buffer at to viewport buffer
		for (int x = 0; x < size.x; x++)
		{
			for (int y = 0; y < size.y; y++)
			{
				m_colorScreenBuffer[y * size.x + x] = buffer[y * size.x + x];
			}
		}
	}
}

void Viewport::SetColorABuffer(Vector2i size, ColorA* buffer)
{
	// If color buffer exists
	if (m_colorScreenBuffer != nullptr)
	{
		// Delete buffer
		delete[] m_colorScreenBuffer;
		this->size = size;

		// Assign new size and buffer
		m_colorScreenBuffer = new Color[size.x * size.y];

		// For each pixel in buffer at to viewport buffer
		for (int x = 0; x < size.x; x++)
		{
			for (int y = 0; y < size.y; y++)
			{
				// Same as AddColorAToBuffer

				// Gets the current color of the viewport and layers the color on top
				Color resultColor = m_colorScreenBuffer[y * size.x + x];
				m_colorScreenBuffer[y * size.x + x] = buffer[y * size.x + x].LayerRGBAOnRGB(resultColor);
			}
		}
	}
}

void Viewport::ClearViewport(bool usePattern)
{
	// For each coordinate in viewport
	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			// Create new color
			Color color = (usePattern) ? ((x + y) % 2 == 0) ? Color(255, 0, 255) : Color(0, 0, 0) : Color(255, 0, 255);
			
			// Set color
			m_colorScreenBuffer[y * size.x + x] = color;

		}
	}
}

Color* Viewport::GetColorScreenBuffer()
{
	return m_colorScreenBuffer;
}

Color& Viewport::GetColorFromLocation(int x, int y)
{
	return m_colorScreenBuffer[y * size.x + x];
}
