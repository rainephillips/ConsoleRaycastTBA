#pragma once

#include "Vector2.h";

class Texture;

class Sprite
{
public:
	Sprite(Vector2 position, Texture* texture);

public:
	Vector2 GetPosition();
	Texture* GetTexture();

private:
	Texture* m_texture;
	Vector2 m_position;
};

