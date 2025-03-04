#pragma once

struct Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2 ONE();

public:
	bool operator==(Vector2& const rhs);
	bool operator!=(Vector2& const rhs);

public:
	float x;
	float y;

};

struct Vector2i
{
public:
	Vector2i();
	Vector2i(int x, int y);
	Vector2i ONE();

public:
	bool operator==(Vector2i& const rhs);
	bool operator!=(Vector2i& const rhs);

public:
	int x;
	int y;

};

