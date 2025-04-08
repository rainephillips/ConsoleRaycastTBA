#pragma once

#include "Vector2.h";

class Texture;

class Sprite
{
public:
	Sprite(Vector2 position, Texture* texture);
	Sprite(Vector2 position, Texture* texture, Vector2 scale, float offset);

public:
	Vector2 GetPosition() const;
	Texture* GetTexture() const;

	Vector2 GetScale() const;
	float GetOffset() const;

	void SetScale(Vector2 scale);
	void SetYOffset(float offset);

private:
	Texture* m_texture;
	Vector2 m_position;

	Vector2 m_uvScale;
	float m_yOffset;
};

