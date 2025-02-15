#pragma once

#include <Windows.h>

struct Vector2i;

#define CLR_BLACK			0
#define CLR_BLUE			1
#define CLR_GREEN			2
#define CLR_CYAN			3
#define CLR_RED				4
#define CLR_MAGENTA			5
#define CLR_BROWN			6
#define CLR_LIGHTGRAY		7
#define CLR_DARKGREY		8
#define CLR_LIGHTBLUE		9
#define CLR_LIGHTGREEN		10
#define CLR_LIGHTCYAN		11
#define CLR_LIGHTRED		12
#define CLR_LIGHTMAGENTA	13
#define CLR_YELLOW			14
#define CLR_WHITE			15

void DrawPoint(int x, int y, char character);

void DrawPoint(int x, int y, char character, unsigned char textColor, unsigned char bgColor);

void DrawRect(int x, int y, int w, int h, char character);

void DrawRect(Vector2i position, Vector2i size, char character);

void DrawVertLine(int x, int start, int end, char character, unsigned char textColor, unsigned char bgColor);

void SetConsoleBufferResolution(unsigned int x, unsigned int y);

void SetConsoleColor(unsigned char textColor, unsigned char bgColor);

void ClearConsoleColor();