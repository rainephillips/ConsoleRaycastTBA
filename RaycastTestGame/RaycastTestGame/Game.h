#pragma once

#include "Color.h";

class Viewport;
class Player;
class Camera;
class Map;

class Game
{
public:
	Game();
	int Run();

public:
	float deltaTime;

private:
	void Raycast(Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, bool useASCII);
	Color GetColorFromRaycast(int x, int y, Map*& map, bool isHorizontal);
	unsigned char GetASCIIColorFromRaycast(int x, int y, Map*& map, bool isHorizontal);

private:
	float m_time;
	float m_oldTime;

	bool gameIsRunning;
};

