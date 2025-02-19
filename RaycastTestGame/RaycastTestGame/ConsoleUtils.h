#pragma once

#include <Windows.h>

struct Vector2i;
class Viewport;

void DrawPoint(int x, int y, char character);

void DrawPoint(int x, int y, char character, unsigned char textColor, unsigned char bgColor);

void DrawRect(int x, int y, int w, int h, char character);

void DrawRect(Vector2i position, Vector2i size, char character);

void SetConsoleBufferResolution(unsigned int x, unsigned int y);

void SetConsoleColor(unsigned char textColor, unsigned char bgColor);

void ClearConsoleColor();

void SetCursorVis(bool visibility);

void DrawASCIIViewport(Viewport* viewport);

void ToggleANSI(bool enabled);

void DrawColorViewport(Viewport* viewport);