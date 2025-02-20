#pragma once

#include "Color.h"
#include "Vector2.h"

class Texture
{
public:
	Texture();
	Texture(Color* image, Vector2i size);
	Texture(Color* image, int sizeX, int sizeY);
	~Texture();

public:
	void SetTexture(Color* image, Vector2i size);
	void CreateNewTexture(Vector2i size);
	void SetTextureColor(int x, int y, Color color);
	Color* GetNewErrorTexture();

	Color* GetTexture();
	Color GetColorFromLocation(int x, int y);
	Vector2i GetSize();

protected:
	Color* m_textureData;
	Vector2i m_size;
};

