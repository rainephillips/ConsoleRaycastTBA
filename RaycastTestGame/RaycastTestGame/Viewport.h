#pragma once

#include <Windows.h>
#include "Vector2.h"
#include "Color.h"

#define CLR_BLACK			0
#define CLR_BLUE			1
#define CLR_GREEN			2
#define CLR_CYAN			3
#define CLR_RED				4
#define CLR_MAGENTA			5
#define CLR_BROWN			6
#define CLR_LIGHTGREY		7
#define CLR_DARKGREY		8
#define CLR_LIGHTBLUE		9
#define CLR_LIGHTGREEN		10
#define CLR_LIGHTCYAN		11
#define CLR_LIGHTRED		12
#define CLR_LIGHTMAGENTA	13
#define CLR_YELLOW			14
#define CLR_WHITE			15

class Viewport
{
public:
	Vector2i position;
	Vector2i size;
	
public:
	Viewport();
	Viewport(Vector2i position, Vector2i size);
	~Viewport();

public:
	void AddColorToBuffer(int x, int y, Color color);
	void AddColorAToBuffer(int x, int y, ColorA color);

	void SetColorBuffer(Vector2i size, Color* buffer);
	void SetColorABuffer(Vector2i size, ColorA* buffer);

	void ClearViewport(bool usePattern);

	Color* GetColorScreenBuffer();

	Color& GetColorFromLocation(int x, int y);

private:
	Color* m_colorScreenBuffer;
};

