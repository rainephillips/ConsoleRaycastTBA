#pragma once

#include <Windows.h>
#include <string>

struct Color;
struct Vector2i;
class Viewport;

using std::string;

// ASCII DRAWING

void DrawPoint(int x, int y, char character);

void DrawPoint(int x, int y, char character, unsigned char textColor, unsigned char bgColor);

void DrawRect(int x, int y, int w, int h, char character);

void DrawRect(Vector2i position, Vector2i size, char character);

void DrawASCIIViewport(Viewport* viewport);

// CONSOLE SETTINGS

void SetConsoleBufferResolution(unsigned int x, unsigned int y);

void SetConsoleColor(unsigned char textColor, unsigned char bgColor);

void ClearConsoleColor();

void SetCursorVis(bool visibility);

void ToggleANSI(bool enabled);

// COLOR DRAWING

void DrawColorViewport(Viewport* viewport);

void CreateColorStringRange(Viewport* viewport, Color* buffer, int yMin, int yMax, int width);