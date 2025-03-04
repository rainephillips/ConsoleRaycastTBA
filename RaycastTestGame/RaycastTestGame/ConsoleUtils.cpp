#include "ConsoleUtils.h"

#include <string>
#include <thread>
#include <vector>

#include "Windows.h"
#include "Vector2.h"
#include "Viewport.h"
#include "Rectangle.h"

using std::string;
using std::vector;
using std::thread;

extern HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

// ASCII Renderings

void DrawPoint(int x, int y, char character)
{
	COORD pos = { (short)x, (short)y };
		
	char charArray[2] = { character };

	SetConsoleCursorPosition(console, pos);
	WriteConsoleA(console, charArray, 1, NULL, NULL);
}

void DrawPoint(int x, int y, char character, unsigned char textColor, unsigned char bgColor)
{
	SetConsoleColor(textColor, bgColor);

	COORD pos = { (short)x, (short)y };

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

// CONSOLE SETTINGS

void SetConsoleCursorPos(short x, short y)
{
	COORD pos = { (short)x, (short)y };

	SetConsoleCursorPosition(console, pos);
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

void ToggleANSI(bool enabled)
{
	DWORD consoleFlags;
	GetConsoleMode(console, &consoleFlags);
	consoleFlags |= (enabled) ? ENABLE_VIRTUAL_TERMINAL_PROCESSING : 0;

	SetConsoleMode(console, consoleFlags);
}

// COLOR RENDERING

void DrawColorViewport(Viewport* viewport)
{
	// Get the 2D Color Array Buffer for the viewport
	Color* buffer = viewport->GetColorScreenBuffer();

	// Create Color Variable to decide the color of each "pixel"
	Color currentColor;

	// Create References for cleaner programming
	int& posX = viewport->position.x;
	int& posY = viewport->position.y;
	int& width = viewport->size.x;
	int& height = viewport->size.y;

	// Create Empty String
	string outputString;
	outputString.reserve(height * (width * 15 + 13));

	int threadCount = 4;
	vector<thread*> threadContainer;
	threadContainer.reserve(threadCount);

	for (int i = 0; i < threadCount; i++)
	{
		threadContainer.emplace_back
		(
			new thread
			(
				CreateColorStringRange, // Function Pointer
				// Parameters
				viewport,
				std::ref(buffer),
				(height / threadCount) * i,
				(height / threadCount) * (i + 1),
				std::ref(outputString)
			)
		);
	}

	for (int i = 0; i < threadCount; i++)
	{
		threadContainer[i]->join();
	}

	for (int i = 0; i < threadCount; i++)
	{
		delete threadContainer[i];
	}

	outputString.append("\033[" + std::to_string(height + posY) + ";0H");
	WriteConsoleA(console, outputString.c_str(), outputString.size(), NULL, NULL);

}

void CreateColorStringRange(Viewport* viewport, Color*& buffer, int yMin, int yMax, string& outputString)
{
	int& width = viewport->size.x;

	string tmpOutputString;
	tmpOutputString.reserve(yMax * (width * 15 + 13));

	// Create Color Variable to decide the color of each "pixel"
	Color currentColor;

	// Create References for cleaner programming
	int& posX = viewport->position.x;
	int& posY = viewport->position.y;

	// For each row
	// THIS PART IS DOOKIE SLOW
	for (int y = yMin; y < yMax; y++)
	{
		// Reposition Cursor using ANSI escape
		tmpOutputString.append("\033[" + std::to_string(y + posY) + ";" + std::to_string(posX) + "H");

		for (int x = 0; x < width; x++)
		{
			// Get Color from y and x cord
			currentColor = buffer[y * width + x];

			// Set a blank ' ' (space) character with the background of the
			// RGB Color value using ANSII Escape
			tmpOutputString.append(currentColor.ToANSIEscape());
		}
	}

	tmpOutputString.append("\033[0m");

	//outputString.append(tmpOutputString);
	WriteConsoleA(console, tmpOutputString.c_str(), tmpOutputString.size(), NULL, NULL);
}


