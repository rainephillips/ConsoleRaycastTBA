#pragma once

#include "Vector2.h"

class Camera
{
public:
	Vector2 size;

public:
	Camera();
	Camera(Vector2 size);
	Camera(float x_size, float y_size);
};

