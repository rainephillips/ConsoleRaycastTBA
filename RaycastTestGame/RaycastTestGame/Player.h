#pragma once

#include "Vector2.h"
#include "Camera.h"

class Player
{
public:
	Vector2 position;
	Vector2 rotation;
	Camera playerCam;

public:
	Player();
};

