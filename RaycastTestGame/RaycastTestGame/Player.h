#pragma once

#include "Camera.h"
#include "Vector2.h"

class Player
{
public:
	Vector2 position;
	Vector2 rotation;
	Camera playerCam;

public:
	Player();
	Player(Vector2 position, Vector2 rotation);
};

