#include "ConsoleUtils.h"


#include <thread>
#include <vector>

#include "Windows.h"
#include "Vector2.h"

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

void SetConsoleViewportResolution( short x,  short y)
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
	DWORD consoleFlags;

	// Add current flags to 
	GetConsoleMode(console, &consoleFlags);

	if (enabled)
		consoleFlags |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	else
		consoleFlags &= ~ENABLE_VIRTUAL_TERMINAL_PROCESSING;

	// Set console flags
	SetConsoleMode(console, consoleFlags);
}

void DrawConsoleViewport(ConsoleViewport* viewport)
{
	WriteConsoleA(console, viewport->data, viewport->size, nullptr, nullptr);
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
