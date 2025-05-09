#include "ConsoleUtils.h"


#include <thread>
#include <vector>

#include "Windows.h"
#include "Vector2.h"
#include "Viewport.h"

using std::vector;
using std::thread;

// Get current console
const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

// CONSOLE SETTINGS

void SetConsoleInfo(Vector2i position, Vector2i size)
{
	// Creates a rectangle containing console boundaries (x1, y1, x2, y,2)
	SMALL_RECT consoleBoundaries =
	{
		static_cast<short>(position.x),
		static_cast<short>(position.y),
		static_cast<short>(position.x + size.x - 1),
		static_cast<short>(position.y + size.y - 1)
	};

	SetConsoleWindowInfo(console, TRUE, &consoleBoundaries);
}

void SetConsoleCursorPos(short x, short y)
{
	COORD pos = { x, y };

	SetConsoleCursorPosition(console, pos);
}

void SetConsoleBufferResolution( short x,  short y)
{
	COORD size = { x, y };
	SetConsoleScreenBufferSize(console, size);
}

void SetConsoleColor(unsigned char textColor, unsigned char bgColor)
{
	// bitwise combine text color and bg color
	SetConsoleTextAttribute(console, textColor | bgColor);
}

void ClearConsoleColor()
{
	// Set console color to black bg and black fg
	SetConsoleTextAttribute(console, 0 | 1);
}

void SetCursorVis(bool visibility)
{
	CONSOLE_CURSOR_INFO cursorInfo{ 0 };
	cursorInfo.bVisible = visibility;
	SetConsoleCursorInfo(console, &cursorInfo);
}

void ToggleANSI(bool enabled)
{
	// Create DWORD var that stores console flags
	DWORD consoleFlags = enabled ? ENABLE_VIRTUAL_TERMINAL_PROCESSING : ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	// Add current flags to 
	GetConsoleMode(console, &consoleFlags);

	// Set console flags
	SetConsoleMode(console, consoleFlags);
}

// COLOR RENDERING

void DrawColorViewport(Viewport* viewport)
{
	// Get the 2D Color Array Buffer for the viewport
	Color* buffer = viewport->GetColorScreenBuffer();

	// Create References for cleaner programming
	int const& posY = viewport->position.y;
	int const& width = viewport->size.x;
	int const& height = viewport->size.y;

	// Create Empty String
	string outputString;
	outputString.reserve(height * (width * 15 + 13));


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
				(height / threadCount) * (i + 1) // Ending point
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
	WriteConsoleA(console, outputString.c_str(), outputString.size(), nullptr, nullptr);

}

void CreateColorStringRange(Viewport* viewport, Color*& buffer, int yMin, int yMax)
{
	int const& width = viewport->size.x;

	string tmpOutputString;
	tmpOutputString.reserve(yMax * (width * 15 + 13));

	// Create References for cleaner programming
	int const& posX = viewport->position.x;
	int const& posY = viewport->position.y;

	// For each row
	// THIS PART IS DOOKIE SLOW
	for (int y = yMin; y < yMax; y++)
	{
		// Reposition Cursor using ANSI escape
		tmpOutputString.append("\033[" + std::to_string(y + posY) + ";" + std::to_string(posX) + "H");

		for (int x = 0; x < width; x++)
		{
			// Get Color from y and x cord
			Color currentColor = buffer[y * width + x];

			// Set a blank ' ' (space) character with the background of the
			// RGB Color value using ANSII Escape
			tmpOutputString.append(currentColor.ToANSIEscape());
		}
	}

	tmpOutputString.append("\033[0m");

	WriteConsoleA(console, tmpOutputString.c_str(), tmpOutputString.size(), nullptr, nullptr);
}

string StringToLower(string const& input)
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

string StringCapitalise(string const& input)
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
