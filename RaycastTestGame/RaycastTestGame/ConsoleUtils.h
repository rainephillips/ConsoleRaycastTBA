#pragma once

#include <Windows.h>
#include <string>

#define PI 3.14159f
#define DEG_TO_RAD(degree) ((degree) * PI / 180.f)

struct Color;
struct Vector2i;
class Viewport;

using std::string;

// CONSOLE SETTINGS

void SetConsoleInfo(Vector2i position, Vector2i size);

void SetConsoleCursorPos(short x, short y);

void SetConsoleBufferResolution( short x,  short y);

void SetConsoleInfo(Vector2i position, Vector2i size);

void SetConsoleColor(unsigned char textColor, unsigned char bgColor);

void ClearConsoleColor();

void SetCursorVis(bool visibility);

void ToggleANSI(bool enabled);

// COLOR DRAWING

void DrawColorViewport(Viewport* viewport);

void CreateColorStringRange(Viewport* viewport, Color*& buffer, int yMin, int yMax);

// STRING UTILITIES

string StringToLower(string const& input);
 
string StringCapitalise(string const& input);