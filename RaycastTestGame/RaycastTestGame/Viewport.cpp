#include "Viewport.h"

#include "Vector2.h"

Viewport::Viewport()
	: position{ 1, 1 }, size{100, 30 }
{
	m_screenBuffer = new CHAR_INFO[size.y * size.x];
}


Viewport::Viewport(Vector2i position, Vector2i size)
	: position{ position }, size { size }
{
	m_screenBuffer = new CHAR_INFO[size.y * size.x];
}

Viewport::~Viewport()
{
	delete[] m_screenBuffer;
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

char Viewport::GetCharFromDepth(float depth)
{
	return charByDepth[int(depth * 91.f)];
}

CHAR_INFO* Viewport::GetScreenBuffer()
{
	return m_screenBuffer;
}
