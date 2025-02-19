#include "Game.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>

#include "ConsoleUtils.h"
#include "Map.h"
#include "Viewport.h"
#include "Player.h"
#include "Texture.h"

using std::vector;

Game::Game()
	: m_oldTime{ 0 }, m_time{ 0 }, gameIsRunning{ true }, deltaTime{ 0.f }
{

}

int Game::Run()
{
	int tempMap[24][24] =
	{
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	Map* map = new Map(24, 24);
	map->SetContents(tempMap, Vector2i(24, 24));

	SetConsoleBufferResolution(2048, 512);
	
	Viewport* mainViewport = new Viewport(Vector2i(10, 6), Vector2i(240, 64));

	Player* player = new Player(Vector2(22.f, 11.f), Vector2(1.f, 0.f));

	Camera* mainCam = new Camera();

	Vector2i defaultTextureSize = Vector2i(64, 64);

	vector<Texture> textureList;
	textureList.reserve(8);

	int& width = mainViewport->size.x;
	int& height = mainViewport->size.y;

	SetCursorVis(false);

	ToggleANSI(true);

	while (gameIsRunning)
	{
		// Timelord shenanigans
		deltaTime = (clock() - m_oldTime) / 1000.f;
		int fps = (1.f / deltaTime);
		m_oldTime = clock();

		
		GetAsyncKeyboardInput(player, mainCam, map);

		Raycast(mainViewport, player, mainCam, map, false);

		DrawColorViewport(mainViewport);
		//DrawASCIIViewport(mainViewport);

	}

	delete player;
	delete mainCam;
	delete mainViewport;
	delete map;

	return EXIT_SUCCESS;
}

void Game::Raycast(Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, bool useASCII)
{
	// Define References for easier code readability

	float& plPosX = player->position.x;
	float& plPosY = player->position.y;

	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	int& width = viewport->size.x;
	int& height = viewport->size.y;

	// Raycasting Loop

	for (int i = 0; i < width; i++)
	{
		//  Right of Screen = 1, Left of Screen = - 1
		float cameraX = 2 * i / (float)width - 1.f; // Camera X Position

		// Set the direction of the ray to the players direction + the x axis of the camera
		Vector2 rayDir = Vector2
		(
			plDirX + camera->size.x * cameraX,
			plDirY + camera->size.y * cameraX
		);

		// Which map cell the Player is in
		Vector2i mapPos = Vector2i
		(
			(int)player->position.x,
			(int)player->position.y
		);

		// Distance to next x or y side
		Vector2 sideDist = Vector2();

		// Distance of ray from one x side or y side to next one
		Vector2 deltaDist = Vector2
		(
			// Prevents Dividing by Zero (i.e. very bad) and returns the closest to infinity instead
			// Abs prevents negative distance (impossible)
			(rayDir.x == 0) ? FLT_MAX : std::abs(1.f / rayDir.x),
			(rayDir.y == 0) ? FLT_MAX : std::abs(1.f / rayDir.y)
		);

		// Will be used to calculate total ray length (Perpendicular Wall Distance)
		float perpWallDist;

		// What direction to make in x or y distance
		Vector2i step = Vector2i();

		bool wallHit = false; // If wall was hit
		bool horizontalWall; // If a NS or EW wall was hit

		// Calculate step and initial sideDist
		if (rayDir.x < 0)
		{
			step.x = -1;
			sideDist.x = (player->position.x - mapPos.x) * deltaDist.x;
		}
		else
		{
			step.x = 1;
			sideDist.x = (mapPos.x + 1.f - player->position.x) * deltaDist.x;
		}
		if (rayDir.y < 0)
		{
			step.y = -1;
			sideDist.y = (player->position.y - mapPos.y) * deltaDist.y;
		}
		else
		{
			step.y = 1;
			sideDist.y = (mapPos.y + 1.f - player->position.y) * deltaDist.y;
		}

		// DDA algorithm (Digital Differential Analyser)
		while (!wallHit)
		{
			// Go to next map square, either in x or y direction
			if (sideDist.x < sideDist.y)
			{
				sideDist.x += deltaDist.x;
				mapPos.x += step.x;
				horizontalWall = false;
			}
			else
			{
				sideDist.y += deltaDist.y;
				mapPos.y += step.y;
				horizontalWall = true;
			}

			// Check if ray has hit a wall
			if (map->contents[mapPos.x][mapPos.y] > 0)
			{
				wallHit = true;
			}
		}

		// Calculate distance of wall projected on camera direction (Direct distance to player would cause fisheye effect)
		if (!horizontalWall)
		{
			perpWallDist = (sideDist.x - deltaDist.x);
		}
		else
		{
			perpWallDist = (sideDist.y - deltaDist.y);
		}

		// Calculate height of line to draw on screen
		int lineHeight = (int)(height / perpWallDist);

		// Calculate the lowest and highest pixel to fill in current line
		// (Center line in middle of viewport)
		int drawStart = -lineHeight / 2 + height / 2;

		// Prevents drawing off screen
		if (drawStart < 0)
		{
			drawStart = 0;
		}

		int drawEnd = lineHeight / 2 + height / 2;
		if (drawEnd >= height)
		{
			// Accounts for Screen starting at 0 and not 1
			drawEnd = height - 1;
		}

		// Checks whether using 24bit color or ASCII Renderer
		if (useASCII)
		{
			// Choose Wall Color
			unsigned char color = GetASCIIColorFromRaycast(mapPos.x, mapPos.y, map, horizontalWall);

			// Get Character
			char pixel = viewport->GetCharFromDepth(float((lineHeight >= height) ? height : lineHeight) / float(height));

			//Add Line to Buffer
			viewport->AddScanlineToBuffer(i, height, drawStart, drawEnd, pixel, color, 0);
		}
		else
		{
			// Get Wall Color
			Color color = GetColorFromRaycast(mapPos.x, mapPos.y, map, horizontalWall);

			// Add Color to buffer
			viewport->AddScanlineToColorBuffer(i, height, drawStart, drawEnd, color);
		}
	}
}

void Game::GetAsyncKeyboardInput(Player*& player, Camera*& camera, Map*& map)
{
	float moveSpeed = deltaTime * 5.0f; // Cells per second
	float rotSpeed = deltaTime * 3.0f; // radians / second

	float& plPosX = player->position.x;
	float& plPosY = player->position.y;

	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	// Keyboard Inputs
	// Move Forward if not crash into wall
	if (GetAsyncKeyState(VK_UP))
	{
		if (map->contents[int(plPosX + plDirX * moveSpeed)][int(plPosY)] == 0)
		{
			plPosX += plDirX * moveSpeed;
		}
		if (map->contents[int(plPosX)][int(plPosY + plDirY * moveSpeed)] == 0)
		{
			plPosY += plDirY * moveSpeed;
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (map->contents[int(plPosX - plDirX * moveSpeed)][int(plPosY)] == 0)
		{
			plPosX -= plDirX * moveSpeed;
		}
		if (map->contents[int(plPosX)][int(plPosY - plDirY * moveSpeed)] == 0)
		{
			plPosY -= plDirY * moveSpeed;
		}
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		float oldDirX = plDirX;
		float oldPLaneX = camera->size.x;

		plDirX = plDirX * cos(-rotSpeed) - plDirY * sin(-rotSpeed);
		plDirY = oldDirX * sin(-rotSpeed) + plDirY * cos(-rotSpeed);

		camera->size.x = camera->size.x * cos(-rotSpeed) - camera->size.y * sin(-rotSpeed);
		camera->size.y = oldPLaneX * sin(-rotSpeed) + camera->size.y * cos(-rotSpeed);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		float oldDirX = plDirX;
		float oldPLaneX = camera->size.x;

		plDirX = plDirX * cos(rotSpeed) - plDirY * sin(rotSpeed);
		plDirY = oldDirX * sin(rotSpeed) + plDirY * cos(rotSpeed);

		camera->size.x = camera->size.x * cos(rotSpeed) - camera->size.y * sin(rotSpeed);
		camera->size.y = oldPLaneX * sin(rotSpeed) + camera->size.y * cos(rotSpeed);
	}
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		gameIsRunning = false;
		return;
	}
}

void Game::CreateDefaultTextures(std::vector<Texture>& textureList, Vector2i textureSize)
{
}

Color Game::GetColorFromRaycast(int x, int y, Map*& map, bool isHorizontal)
{
	switch (map->contents[x][y])
	{
	case 1:
	{
		return (isHorizontal) ? Color(179, 39, 29) : Color(235, 64, 52);
	}
	case 2:
	{
		return (isHorizontal) ? Color(50, 168, 82) : Color(44, 222, 93);
	}
	case 3:
	{
		return (isHorizontal) ? Color(66, 135, 245) : Color(142, 245, 250);
	}
	case 4:
	{
		return (isHorizontal) ? Color(173, 28, 159) : Color(250, 112, 236);
	}
	default:
	{
		return (isHorizontal) ? Color(80, 90, 99) : Color(195, 213, 230);
	}
	}
}

unsigned char Game::GetASCIIColorFromRaycast(int x, int y, Map*& map, bool isHorizontal)
{
	switch (map->contents[x][y])

	{
	case 1:
	{
		return (isHorizontal) ? CLR_RED : CLR_LIGHTRED;
	}
	case 2:
	{
		return (isHorizontal) ? CLR_GREEN : CLR_LIGHTGREEN;
	}
	case 3:
	{
		return (isHorizontal) ? CLR_CYAN : CLR_LIGHTCYAN;
	}
	case 4:
	{
		return (isHorizontal) ? CLR_MAGENTA : CLR_LIGHTMAGENTA;
	}
	default:
	{
		return (isHorizontal) ? CLR_DARKGREY : CLR_WHITE;
	}
	}
}
