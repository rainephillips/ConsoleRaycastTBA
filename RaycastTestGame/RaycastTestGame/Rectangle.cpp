#include "Rectangle.h"

Rectangle::Rectangle(float x, float y, float w, float h)
	: position{ x, y }, scale{ w, h }
{
}

void Rectangle::SetPos(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Rectangle::SetPos(Vector2 pos)
{
	position = pos;
}

void Rectangle::SetSize(float x, float y)
{
	scale.x = x;
	scale.y = y;
}

void Rectangle::SetSize(Vector2 size)
{
	scale = size;
}
