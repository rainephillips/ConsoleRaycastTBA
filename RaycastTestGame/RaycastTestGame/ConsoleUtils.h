#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <thread>

#define PI 3.14159f
#define DEG_TO_RAD(degree) ((degree) * PI / 180.f)

struct Color;
struct Vector2i;
class Viewport;

using std::string;
using std::vector;
using std::thread;

// ASCII DRAWING

void DrawPoint(int x, int y, char character);

void DrawPoint(int x, int y, char character, unsigned char textColor, unsigned char bgColor);

void DrawRect(int x, int y, int w, int h, char character);

void DrawRect(Vector2i position, Vector2i size, char character);

void DrawASCIIViewport(Viewport* viewport);

// CONSOLE SETTINGS

void SetConsoleCursorPos(short x, short y);

void SetConsoleBufferResolution(unsigned int x, unsigned int y);

void SetConsoleColor(unsigned char textColor, unsigned char bgColor);

void ClearConsoleColor();

void SetCursorVis(bool visibility);

void ToggleANSI(bool enabled);

// COLOR DRAWING

void DrawColorViewport(Viewport* viewport);

void CreateColorStringRange(Viewport* viewport, Color*& buffer, int yMin, int yMax, string& outputString);

// STRING UTILITIES

string StringToLower(string input);

string StringCapitalise(string input);