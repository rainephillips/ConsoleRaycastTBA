#include "Camera.h"

const float DEFAULT_FOV = 0.9f;

Camera::Camera()
	: size{ 0.f, DEFAULT_FOV }
{
	m_viewport.width = 128;
	m_viewport.height = 128;
	m_viewport.x = 0;
	m_viewport.y = 0;

	CreateConsoleViewport(&m_viewport);
}

Camera::Camera(Vector2 size, Vector2i viewportPos, Vector2i viewportSize)
	: size{ size }
{
	m_viewport.width = viewportSize.x;
	m_viewport.height = viewportSize.y;
	m_viewport.x = viewportPos.x;
	m_viewport.y = viewportPos.y;

	CreateConsoleViewport(&m_viewport);
}

Camera::Camera(float xSize, float ySize, int viewportXPos, int viewportYPos, int viewportXSize, int viewportYSize)
	: size{ Vector2( xSize, ySize ) }
{
	m_viewport.width = viewportXSize;
	m_viewport.height = viewportYSize;
	m_viewport.x = viewportXPos;
	m_viewport.y = viewportYPos;

	CreateConsoleViewport(&m_viewport);
}

Camera::~Camera()
{
	ClearConsoleViewport(&m_viewport);
}

ConsoleViewport* Camera::GetViewport()
{
	return &m_viewport;
}
