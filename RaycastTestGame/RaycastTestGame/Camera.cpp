#include "Camera.h"

Camera::Camera()
	: size{ 0.f, 0.66f }
{
}

Camera::Camera(Vector2 size)
	: size{ size }
{
}

Camera::Camera(float x_size, float y_size)
	: size{ Vector2( x_size, y_size ) }
{
}
