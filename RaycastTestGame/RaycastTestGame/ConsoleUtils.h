#pragma once

#include <Windows.h>

#include "Vector2.h"

void drawPoint(int x, int y, char character);

void drawRect(int x, int y, int w, int h, char character);

void drawRect(Vector2i position, Vector2i size, char character);

void setConsoleBufferResolution(unsigned int x, unsigned int y);