#pragma once

#include "Vector2.h"

class Viewport;

class Camera
{
public:
	Camera();
	Camera(Vector2 size, Vector2i viewportPos, Vector2i viewportSize);
	Camera(float xSize, float ySize, int viewportXPos, int viewportYPos, int viewportXSize, int viewportYSize);
	~Camera();

public:
	Viewport* GetViewport();

public:
	Vector2 size;

private:
	Viewport* m_viewport;
};

