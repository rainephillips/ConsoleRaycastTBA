#pragma once

#include "ConsoleUtils.h";

class Viewport
{
public:
	Vector2i position;
	Vector2i size;

public:
	Viewport();
	Viewport(Vector2i position, Vector2i size);

	void Clear();
	void Fill(char character);
};

