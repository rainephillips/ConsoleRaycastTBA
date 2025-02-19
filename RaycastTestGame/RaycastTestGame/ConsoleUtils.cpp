#include "ConsoleUtils.h"

#include <string>

#include "Windows.h"
#include "Vector2.h"
#include "Viewport.h"
#include "Rectangle.h"

using std::string;

extern HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void DrawPoint(int x, int y, char character)
{
	COORD pos = { x, y };

	char charArray[2] = { character };

	SetConsoleCursorPosition(console, pos);
	WriteConsoleA(console, charArray, 1, NULL, NULL);
}

void DrawPoint(int x, int y, char character, unsigned char textColor, unsigned char bgColor)
{
	SetConsoleColor(textColor, bgColor);

	COORD pos = { x, y };

	char charArray[2] = { character };

	SetConsoleCursorPosition(console, pos);
	WriteConsole(console, charArray, 1, NULL, NULL);

	ClearConsoleColor();
}

void DrawRect(int x, int y, int w, int h, char character)
{
	for(int i = 0; i < h; ++i)
	{ 
		for (int j = 0; j < w; ++j)
		{
			DrawPoint(j + x, i + y, character);
		}
	}
}

void DrawRect(Vector2i position, Vector2i size, char character)
{
	DrawRect(position.x, position.y, size.x, size.y, character);
}

void DrawVertLine(int x, int height, int start, int end, char character, unsigned char textColor, unsigned char bgColor)
{
	SetConsoleColor(textColor, bgColor);
	for (int i = 0; i < height; i++)
	{
		if (i < start)
		{
			DrawPoint(x, i, ' ');
		}
		else if (i > end)
		{
			SetConsoleColor(CLR_BROWN, bgColor);
			DrawPoint(x, i, '`');
		}
		else 
		{
			SetConsoleColor(textColor, bgColor);
			DrawPoint(x, i, character);
		}
		
	}
	ClearConsoleColor();
}

void SetConsoleBufferResolution(unsigned int x, unsigned int y)
{
	COORD size = { x, y };
	SetConsoleScreenBufferSize(console, size);
}

void SetConsoleColor(unsigned char textColor, unsigned char bgColor)
{
	SetConsoleTextAttribute(console, (textColor) | bgColor);
}

void ClearConsoleColor()
{
	SetConsoleTextAttribute(console, (0) | 1);
}

void SetCursorVis(bool visibility)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = visibility;
	cursorInfo.dwSize = 20;
	SetConsoleCursorInfo(console, &cursorInfo);
}

void DrawColorViewport(Viewport* viewport)
{
	Color* buffer = viewport->GetColorScreenBuffer();
	Color currentColor;

	string outputString = "";
	int& posX = viewport->position.x;
	int& posY = viewport->position.y;
	int& width = viewport->size.x;
	int& height = viewport->size.y;
	
	for (int y = 0; y < height; y++)
	{
		outputString += ("\033[" + std::to_string(posX) + ";" + std::to_string(y + posY) + "f");

		for (int x = 0; x < width; x++)
		{
			currentColor = buffer[y * width + x];
			outputString += ("\033[48;2;" 
				+ currentColor.ToStringValue(currentColor.r) + ";"
				+ currentColor.ToStringValue(currentColor.g) + ";"
				+ currentColor.ToStringValue(currentColor.b) + "m ");
		}
		outputString += "\033[0m"; // Reset Color
	}

	WriteConsoleA(console, outputString.c_str(), outputString.size(), NULL, NULL);
}

void DrawASCIIViewport(Viewport* viewport)
{
	int& width = viewport->size.x;
	int& height = viewport->size.y;

	CHAR_INFO* buffer = viewport->GetASCIIScreenBuffer();

	COORD bufferSize = { width, height };
	COORD bufferPos = { viewport->position.x, viewport->position.y };
	SMALL_RECT bufferRect = { 
		viewport->position.x,
		viewport->position.y,
		viewport->position.x + width - 1,
		viewport->position.y + height - 1};

	WriteConsoleOutputA(console, buffer, bufferSize, bufferPos, &bufferRect);

}

void ToggleANSI(bool enabled)
{
	DWORD consoleFlags = 0;
	consoleFlags |= (enabled) ? ENABLE_VIRTUAL_TERMINAL_PROCESSING : 0;

	SetConsoleMode(console, consoleFlags);
}
