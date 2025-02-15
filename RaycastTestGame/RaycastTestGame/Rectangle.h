#pragma once

#include "Vector2.h"

class Rectangle
{
public:
	Rectangle(float x, float y, float w, float h);

	void SetPos(float x, float y);
	void SetPos(Vector2 pos);

	void SetSize(float x, float y);
	void SetSize(Vector2 size);

public:
	Vector2 position;
	Vector2 scale;
};