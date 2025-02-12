#include "Viewport.h"

Viewport::Viewport()
	: position{ 1, 1 }, size{ 80, 60 }
{
}


Viewport::Viewport(Vector2i position, Vector2i size)
	: position{ position }, size { size }
{
}

void Viewport::Clear()
{
	drawRect(position, size, ' ');
}

void Viewport::Fill(char character)
{
	drawRect(position, size, '#');
}
