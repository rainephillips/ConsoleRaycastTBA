#include "ConsoleUtils.h"

void drawPoint(int x, int y, char character)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };

	char charArray[2] = { character };

	SetConsoleCursorPosition(console, pos);
	WriteConsole(console, charArray, 1, NULL, NULL);
}

void drawRect(int x, int y, int w, int h, char character)
{
	for(int i = 0; i < h; ++i)
	{ 
		for (int j = 0; j < w; ++j)
		{
			drawPoint(j + x, i + y, character);
		}
	}
}

void setConsoleBufferResolution(unsigned int x, unsigned int y)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD size = { x, y };
	SetConsoleScreenBufferSize(console, size);
}
