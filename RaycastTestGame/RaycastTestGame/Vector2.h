#pragma once
struct Vector2
{
public:
	float x;
	float y;

public:
	Vector2();
	Vector2(float x, float y);
	Vector2 ONE();

};

struct Vector2i
{
public:
	int x;
	int y;
public:
	Vector2i();
	Vector2i(int x, float y);
	Vector2i ONE();
};

