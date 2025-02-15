#include "Player.h"

#include "Vector2.h"
#include "Camera.h"

Player::Player()
	: position{ 0.f, 0.f }, rotation{ -1.f, 0.f }
{
}

Player::Player(Vector2 position, Vector2 rotation)
	: position{ position }, rotation{ rotation }
{
}
