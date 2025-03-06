#include "Viewport.h"

#include "Vector2.h"

Viewport::Viewport()
	: position{ 1, 1 }, size{100, 30 }
{
	m_screenBuffer = new CHAR_INFO[size.y * size.x];
	m_colorScreenBuffer = new Color[size.y * size.x];
}


Viewport::Viewport(Vector2i position, Vector2i size)
	: position{ position }, size { size }
{
	m_screenBuffer = new CHAR_INFO[size.y * size.x];
	m_colorScreenBuffer = new Color[size.y * size.x];
}

Viewport::~Viewport()
{
	if (m_screenBuffer != nullptr)
	{
		delete[] m_screenBuffer;
	}
	if (m_colorScreenBuffer != nullptr)
	{
		delete[] m_colorScreenBuffer;
	}
}

extern const char charByDepth[93] = { " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@" };

void Viewport::AddScanlineToBuffer(int x, int height, int start, int end, char character, WORD textColor, WORD bgColor)
{
	for (int i = 0; i < height; i++)
	{
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
	CHAR_INFO charInfo;
	charInfo.Char.AsciiChar = (character);
	charInfo.Attributes = textColor | bgColor;
	m_screenBuffer[y * size.x + x] = charInfo;
}

void Viewport::AddColorToBuffer(int x, int y, Color color)
{
	m_colorScreenBuffer[y * size.x + x] = color;
}

void Viewport::AddColorAToBuffer(int x, int y, ColorA color)
{
	Color resultColor = m_colorScreenBuffer[y * size.x + x];
	m_colorScreenBuffer[y * size.x + x] = color.LayerRGBAOnRGB(resultColor);
}

void Viewport::AddScanlineToColorBuffer(int x, int height, int start, int end, Color color)
{
	for (int i = start; i < end; i++)
	{
		AddColorToBuffer(x, i, color);
	}
}

void Viewport::SetColorBuffer(Vector2i size, Color* buffer)
{
	if (m_colorScreenBuffer != nullptr)
	{
		delete[] m_colorScreenBuffer;
		this->size = size;
		m_colorScreenBuffer = new Color[size.x * size.y];

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
	if (m_colorScreenBuffer != nullptr)
	{
		delete[] m_colorScreenBuffer;
		this->size = size;
		m_colorScreenBuffer = new Color[size.x * size.y];

		for (int x = 0; x < size.x; x++)
		{
			for (int y = 0; y < size.y; y++)
			{
				Color resultColor = m_colorScreenBuffer[y * size.x + x];
				m_colorScreenBuffer[y * size.x + x] = buffer[y * size.x + x].LayerRGBAOnRGB(resultColor);
			}
		}
	}
}

void Viewport::ClearViewport(bool usePattern)
{
	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			Color color;
			if (usePattern)
			{
				color = ((x + y) % 2 == 0) ? Color(255, 0, 255) : Color(0, 0, 0);
			}
			else
			{
				color = Color(255, 0, 255);
			}

			m_colorScreenBuffer[y * size.x + x] = color;

		}
	}
}

char Viewport::GetCharFromDepth(float depth)
{
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
