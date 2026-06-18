#pragma once

#include "Vector2.h"

#include "render/buffer/ConsolePixelBuffer.h"

class Camera
{
public:
	Camera();
	Camera(Vector2 size, Vector2i viewportPos, Vector2i viewportSize);
	Camera(float xSize, float ySize, int viewportXPos, int viewportYPos, int viewportXSize, int viewportYSize);
	~Camera();

public:
	ConsoleViewport* GetViewport();

public:
	Vector2 size;

private:
	ConsoleViewport m_viewport;
};

