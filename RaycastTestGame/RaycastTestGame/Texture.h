#pragma once

#include "Color.h"
#include "Vector2.h"

class Texture
{
public:
	Texture();
	Texture(ColorA* image, Vector2i size);
	Texture(ColorA* image, int sizeX, int sizeY);
	Texture(const char* filepath);
	~Texture();

public:
	void SetTexture(ColorA* image, Vector2i size);
	void SetTexture(const char* filepath);
	void CreateNewTexture(Vector2i size);
	void SetTextureColor(int x, int y, ColorA color);
	ColorA* GetNewErrorTexture();

	ColorA* GetTexture();
	ColorA GetColorFromLocation(int x, int y) const;
	Vector2i GetSize() const;

private:
	ColorA* m_textureData;
	Vector2i m_size;
};

