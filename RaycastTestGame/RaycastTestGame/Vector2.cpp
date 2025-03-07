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

bool Vector2::operator==(Vector2& const rhs)
{
	return (x == rhs.x && y == rhs.y) ? true : false;
}

bool Vector2::operator!=(Vector2& const rhs)
{
	return (x == rhs.x && y == rhs.y) ? false : true;
}

const Vector2& Vector2::operator+(const Vector2& rhs)
{
	return Vector2(x + rhs.x, y + rhs.y);
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

bool Vector2i::operator==(Vector2i& const rhs)
{
	return (x == rhs.x && y == rhs.y) ? true : false;
}

bool Vector2i::operator!=(Vector2i& const rhs)
{
	return (x == rhs.x && y == rhs.y) ? false : true;
}

const Vector2i& Vector2i::operator+(const Vector2i& rhs)
{
	return Vector2i(x + rhs.x, y + rhs.y);
}
