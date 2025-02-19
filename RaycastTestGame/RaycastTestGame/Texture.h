#pragma once

#include "Color.h"
#include "Vector2.h"

Color errorTexture[] = { Color(255,0,255), Color(0,0,0), 
					Color(255,0,255), Color(0,0,0) };

class Texture
{
public:
	Texture();
	Texture(Color* image, Vector2i size);
	Texture(Color* image, int sizeX, int sizeY);
	~Texture();

public:
	void SetTexture(Color* image, Vector2i size);
	Color* GetTexture();
	Vector2i GetSize();

protected:
	Color* m_textureData;
	Vector2i m_size;
};

