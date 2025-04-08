#pragma once

struct Vector2
{
public:
	float x;
	float y;

public:
	Vector2();
	Vector2(float x, float y);

public:
	bool operator==(Vector2& const rhs);
	bool operator!=(Vector2& const rhs);

	const Vector2 operator+(const Vector2& rhs);

public:
	Vector2 ONE();

};

struct Vector2i
{

public:
	int x;
	int y;

public:
	Vector2i();
	Vector2i(int x, int y);

public:
	bool operator==(Vector2i& const rhs);
	bool operator!=(Vector2i& const rhs);

	const Vector2i operator+(const Vector2i& rhs);

public:
	Vector2i ONE();

};

