#pragma once

#include "Vector2.h";

class TextureA;

struct Sprite
{
public:
	Sprite();
	Sprite(Vector2 position, TextureA* texture);

public:
	Vector2 GetPosition();
	TextureA* GetTexture();


private:
	TextureA* m_texture;
	Vector2 m_position;
};

