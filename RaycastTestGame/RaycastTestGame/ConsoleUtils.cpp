#include "ConsoleUtils.h"

#include <cmath>
#include <string>
#include <thread>
#include <vector>

#include "Windows.h"
#include "Vector2.h"
#include "Viewport.h"

using std::string;
using std::vector;
using std::thread;

// Get current console
extern HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

// ASCII Renderings 

void DrawPoint(int x, int y, char character)
{
	// Convert x, y into COORD
	COORD pos = { (short)x, (short)y };
	
	// Create char array to output (character + null terminator)
	char charArray[2] = { character };

	SetConsoleCursorPosition(console, pos);

	// Output to console
	WriteConsoleA(console, charArray, 1, NULL, NULL);
}

void DrawPoint(int x, int y, char character, unsigned char textColor, unsigned char bgColor)
{
	// Set console color to text + background color
	SetConsoleColor(textColor, bgColor);
	
	// Convert x, y into COORD
	COORD pos = { (short)x, (short)y };

	char charArray[2] = { character };
	
	// Create char array to output (character + null terminator)
	SetConsoleCursorPosition(console, pos);
	WriteConsole(console, charArray, 1, NULL, NULL);

	// Clear console color
	ClearConsoleColor();
}

void DrawRect(int x, int y, int w, int h, char character)
{
	// Scanline drawing of a rectangle
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
	// ORIGINALLY USED FOR ASCII RENDERED THAT DIDN'T HAVE ROOF AND FLOOR SUPPORT
	SetConsoleColor(textColor, bgColor);
	for (int i = 0; i < height; i++)
	{
		// If position above wall
		if (i < start)
		{
			DrawPoint(x, i, ' ');
		}
		// If position below wall
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

void SetConsoleInfo(Vector2i position, Vector2i size)
{
	// Creates a rectangle containing console boundaries (x1, y1, x2, y,2)
	SMALL_RECT consoleBoundaries =
	{
		short(position.x),
		short(position.y),
		short(position.x + size.x - 1),
		short(position.y + size.y - 1)
	};

	SetConsoleWindowInfo(console, TRUE, &consoleBoundaries);
}

void SetConsoleCursorPos(short x, short y)
{
	COORD pos = { (short)x, (short)y };

	SetConsoleCursorPosition(console, pos);
}

void SetConsoleBufferResolution(unsigned short x, unsigned short y)
{
	COORD size = { x, y };
	SetConsoleScreenBufferSize(console, size);
}

void SetConsoleColor(unsigned char textColor, unsigned char bgColor)
{
	// bitwise combine text color and bg color
	SetConsoleTextAttribute(console, (textColor) | bgColor);
}

void ClearConsoleColor()
{
	// Set console color to black bg and black fg
	SetConsoleTextAttribute(console, (0) | 1);
}

void SetCursorVis(bool visibility)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = visibility;
	SetConsoleCursorInfo(console, &cursorInfo);
}

void ToggleANSI(bool enabled)
{
	// Create DWORD var that stores console flags
	DWORD consoleFlags;

	// Add current flags to 
	GetConsoleMode(console, &consoleFlags);

	// Set console flags to either ANSI escape support or not
	consoleFlags |= (enabled) ? ENABLE_VIRTUAL_TERMINAL_PROCESSING : ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	// Set console flags
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

	//CreateColorStringRange(viewport, buffer, 0, height, outputString);

	// Create thread vector container threadcount amount of threads and reserve that size

	// The threads render 1/threadcount the viewport (ie 4 would mean each thread handles 1/4 of the viewport)
	int threadCount = 4;
	vector<thread*> threadContainer;
	threadContainer.reserve(threadCount);

	// For each thread
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
				(height / threadCount) * i, // Starting point
				(height / threadCount) * (i + 1), // Ending point
				std::ref(outputString)
			)
		);
	}

	for (int i = 0; i < threadCount; i++)
	{
		threadContainer[i]->join(); // Wait for all threads to finish
	}

	for (int i = 0; i < threadCount; i++)
	{
		delete threadContainer[i]; // Delete thread
	}

	// Empty thread container
	threadContainer.clear();

	// Reset color
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

string StringToLower(string input)
{
	// Create output string
	string output = input;

	// For each character
	for (int i = 0; i < output.size(); i++)
	{
		// If uppercase alphabet convert to lowercase
		if (output[i] >= 'A' && output[i] <= 'Z')
		{
			output[i] += 32;
		}
	}

	return output;
}

string StringCapitalise(string input)
{
	// Create output string
	string output = input;

	// If its looking to capitalise character
	bool changeChar = true;

	// For each character
	for (char i = 0; i < output.size(); i++)
	{
		if (changeChar)
		{
			// If lowercase make uppercase and disable capitalising the next character
			if (output[i] >= 'a' && output[i] <= 'z')
			{
				output[i] -= 32;
				changeChar = false;
			}
		}
		else
		{
			// If '-', '_', or ' ' convert next alphabetical character to uppecase
			switch (output[i])
			{
				case ' ':
				{
					changeChar = true;
					break;
				}

				case '-':
				{
					changeChar = true;
					break;
				}

				case '_':
				{
					changeChar = true;
					break;
				}

				default:
				{
					break;
				}
			}
		}
	}

	return output;
}
