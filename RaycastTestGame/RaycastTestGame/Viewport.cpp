#include "Viewport.h"

#include "ConsoleUtils.h"
#include "Vector2.h"

Viewport::Viewport()
	: position{ 1, 1 }, size{100, 30 }
{
}


Viewport::Viewport(Vector2i position, Vector2i size)
	: position{ position }, size { size }
{
}

void Viewport::Clear()
{
	DrawRect(position, size, ' ');
}

void Viewport::Fill(char character)
{
	DrawRect(position, size, '#');
}
