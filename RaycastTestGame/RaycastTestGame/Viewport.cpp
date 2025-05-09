#include "Viewport.h"

#include "Vector2.h"

const Vector2i viewportDefaultPosition = Vector2i{ 0, 0 };
const Vector2i viewportDefaultSize = Vector2i{ 100, 30 };

Viewport::Viewport()
	: position{ viewportDefaultPosition.x, viewportDefaultPosition.y }, size{ viewportDefaultSize.x, viewportDefaultSize.y }
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
	Color resultColor = color.LayerRGBAOnRGB(m_colorScreenBuffer[y * size.x + x]);
	m_colorScreenBuffer[y * size.x + x] = resultColor;
}

void Viewport::SetColorBuffer(Vector2i bufferSize, Color* buffer)
{
	// If color buffer exists
	if (m_colorScreenBuffer != nullptr)
	{
		// Delete buffer
		delete[] m_colorScreenBuffer;

		// Assign new size and buffer
		size = bufferSize;
		m_colorScreenBuffer = new Color[bufferSize.x * bufferSize.y];

		// For each pixel in buffer at to viewport buffer
		for (int x = 0; x < size.x; x++)
		{
			for (int y = 0; y < size.y; y++)
			{
				m_colorScreenBuffer[y * size.x + x] = buffer[y * bufferSize.x + x];
			}
		}
	}
}

void Viewport::SetColorABuffer(Vector2i bufferSize, ColorA* buffer)
{
	// If color buffer exists
	if (m_colorScreenBuffer != nullptr)
	{
		// Delete buffer
		delete[] m_colorScreenBuffer;
		size = bufferSize;

		// Assign new size and buffer
		m_colorScreenBuffer = new Color[bufferSize.x * bufferSize.y];

		// For each pixel in buffer at to viewport buffer
		for (int x = 0; x < size.x; x++)
		{
			for (int y = 0; y < size.y; y++)
			{

				// Gets the current color of the viewport and layers the color on top
				Color resultColor = buffer[y * size.x + x].LayerRGBAOnRGB(m_colorScreenBuffer[y * size.x + x]);
				m_colorScreenBuffer[y * size.x + x] = resultColor;
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
			Color color = usePattern ? ((x + y) % 2 == 0) ? Color(255, 0, 255) : Color(0, 0, 0) : Color(255, 0, 255);
			
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
