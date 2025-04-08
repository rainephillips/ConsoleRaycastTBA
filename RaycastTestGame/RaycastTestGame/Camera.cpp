#include "Camera.h"

#include "Viewport.h"

const float DEFAULT_FOV = 0.9f;

Camera::Camera()
	: size{ 0.f, DEFAULT_FOV }, m_viewport{new Viewport()}
{
}

Camera::Camera(Vector2 size, Vector2i viewportPos, Vector2i viewportSize)
	: size{ size }, m_viewport{ new Viewport(viewportPos, viewportSize)}
{
}

Camera::Camera(float xSize, float ySize, int viewportXPos, int viewportYPos, int viewportXSize, int viewportYSize)
	: size{ Vector2( xSize, ySize ) }, 
	m_viewport{ new Viewport(Vector2i(viewportXPos, viewportYPos), Vector2i(viewportXSize, viewportYSize))}
{
}

Camera::~Camera()
{
	delete m_viewport;
}

Viewport* Camera::GetViewport()
{
	return m_viewport;
}
