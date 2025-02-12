#include "Vector2.h"

Vector2::Vector2()
	: x{ 0 }, y{ 0 }
{
}

Vector2::Vector2(float x, float y)
	: x{ x }, y{ y }
{
}

Vector2 Vector2::ONE()
{
	return Vector2(1.f, 1.f);
}

/*
	VECTOR2i
*/

Vector2i::Vector2i()
	: x{ 0 }, y{ 0 }
{
}

Vector2i::Vector2i(int x, int y)
	: x{ x }, y{ y }
{
}

Vector2i Vector2i::ONE()
{
	return Vector2i(1, 1);
}
