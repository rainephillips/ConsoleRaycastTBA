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
	// Get the 2D Color Array Buffer for the viewport
	Color* buffer = viewport->GetColorScreenBuffer();

	// Create Color Variable to decide the color of each "pixel"
	Color currentColor;

	// Create Empty String
	string outputString;

	// Create References for cleaner programming
	int& posX = viewport->position.x;
	int& posY = viewport->position.y;
	int& width = viewport->size.x;
	int& height = viewport->size.y;

	//Preallocate Estimated amount of memory for more performance to prevent creating multiple more arrays
	// "\033[255;255;255m " has a total of 15 character but also have to make room for mouse repositioning
	outputString.reserve(height * (width * 15 + 12));

	
	
	// For each row
	for (int y = 0; y < height; y++)
	{
		// Reposition Cursor using ANSI escape
		outputString.append("\033[" + std::to_string(y + posY) + ";" + std::to_string(posX) + "H");

		// For each column in row
		for (int x = 0; x < width; x++)
		{
			// Get Color from y and x cord
			currentColor = buffer[y * width + x];

			// Set a blank ' ' (space) character with the background of the
			// RGB Color value using ANSII Escape
			outputString.append("\033[48;2;"
				+ std::to_string(static_cast<short>(currentColor.r)) + ";"
				+ std::to_string(static_cast<short>(currentColor.g)) + ";"
				+ std::to_string(static_cast<short>(currentColor.b)) + "m ");
		}

	}
	// Reset Color on Text
	outputString.append("\033[0m");

	//Get final output without any overhead data
	string finalOutput = outputString.c_str();

	// Output to the console the final string with the total length of the string
	WriteConsoleA(console, finalOutput.c_str(), outputString.size(), NULL, NULL);
}

void DrawASCIIViewport(Viewport* viewport)
{

	// Create References for cleaner programming
	int& width = viewport->size.x;
	int& height = viewport->size.y;

	// Get 2D char info array from viewport
	CHAR_INFO* buffer = viewport->GetASCIIScreenBuffer();

	COORD bufferSize = { width, height }; // Set Scale of output  
	COORD bufferPos = { viewport->position.x, viewport->position.y }; // Set Position of Output
	SMALL_RECT bufferRect = {  // Set Rectangle Boundary of Output
		viewport->position.x,
		viewport->position.y,
		viewport->position.x + width - 1,
		viewport->position.y + height - 1};

	// Ouput Data to Console
	WriteConsoleOutputA(console, buffer, bufferSize, bufferPos, &bufferRect);

}

void ToggleANSI(bool enabled)
{
	DWORD consoleFlags;
	GetConsoleMode(console, &consoleFlags);
	consoleFlags |= (enabled) ? ENABLE_VIRTUAL_TERMINAL_PROCESSING : 0;

	SetConsoleMode(console, consoleFlags);
}
