#include "Viewport.h"

#include "Vector2.h"

Viewport::Viewport()
	: position{ 1, 1 }, size{100, 30 }
{
	// Create new ascii and color screen buffer with the size of the viewport
	m_screenBuffer = new CHAR_INFO[size.y * size.x];
	m_colorScreenBuffer = new Color[size.y * size.x];
}


Viewport::Viewport(Vector2i position, Vector2i size)
	: position{ position }, size { size }
{
	// Create new ascii and color screen buffer with the size of the viewport
	m_screenBuffer = new CHAR_INFO[size.y * size.x];
	m_colorScreenBuffer = new Color[size.y * size.x];
}

Viewport::~Viewport()
{
	// If buffer exists delete it
	if (m_screenBuffer != nullptr)
	{
		delete[] m_screenBuffer;
	}
	if (m_colorScreenBuffer != nullptr)
	{
		delete[] m_colorScreenBuffer;
	}
}

void Viewport::AddScanlineToBuffer(int x, int height, int start, int end, char character, WORD textColor, WORD bgColor)
{
	// MEANT FOR ASCII RENDERING
	for (int i = 0; i < height; i++)
	{
		// IF CHARACTER IS ABOVE WALL: BLACK EMPTY
		// IF BELOW BROWN '`'
		// ELSE OUTPUT COLOR CHARACTER AND BACKROUND COLOR
		if (i < start)
		{
			AddCharToBuffer(x, i, ' ', textColor, bgColor);
		}
		else if (i > end)
		{
			AddCharToBuffer(x, i, '`', CLR_BROWN, bgColor);
		}
		else
		{
			AddCharToBuffer(x, i, character, textColor, bgColor);
		}

	}
}

void Viewport::AddCharToBuffer(int x, int y, char character, WORD textColor, WORD bgColor)
{
	// Create new char info var (stores character + color + bg color + other properties'
	CHAR_INFO charInfo;

	// Add it as an ascii character
	charInfo.Char.AsciiChar = (character);

	// Add background and text color bitwise combined into attributes
	charInfo.Attributes = textColor | bgColor;

	// Ad char info to buffer
	m_screenBuffer[y * size.x + x] = charInfo;
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

void Viewport::AddScanlineToColorBuffer(int x, int height, int start, int end, Color color)
{
	// USED FOR OLD SOLID COLOR RAYCASTING
	// For the draw length add color to buffer
	for (int i = start; i < end; i++)
	{
		AddColorToBuffer(x, i, color);
	}
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
			Color color;
			
			// If using checkerboard pattern
			if (usePattern)
			{
				// If x + y is even to purple else do black (create checkerboard pattern)
				color = ((x + y) % 2 == 0) ? Color(255, 0, 255) : Color(0, 0, 0);
			}
			else
			{
				color = Color(255, 0, 255);
			}

			// Set color
			m_colorScreenBuffer[y * size.x + x] = color;

		}
	}
}

// FOR ASCII RENDERING
// A LIST OF CHARACTERS ORDERED BY COVERAGE OF CELL
// TAKEN FROM "https://stackoverflow.com/questions/30097953/"

extern const char charByDepth[93] = { " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@" };

char Viewport::GetCharFromDepth(float depth)
{
	// Return depth by converting the 0-1 depth into a range in the array to take
	// Technically inaccurate if there were large differences in shading detail
	// But would require more computing for minimal return
	return charByDepth[int(depth * 91.f)];
}

CHAR_INFO* Viewport::GetASCIIScreenBuffer()
{
	return m_screenBuffer;
}

Color* Viewport::GetColorScreenBuffer()
{
	return m_colorScreenBuffer;
}

Color& Viewport::GetColorFromLocation(int x, int y)
{
	return m_colorScreenBuffer[y * size.x + x];
}
