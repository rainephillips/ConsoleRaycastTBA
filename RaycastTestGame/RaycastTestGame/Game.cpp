#include "Game.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <stdlib.h> 

#include "ConsoleUtils.h"
#include "Map.h"
#include "Viewport.h"
#include "Player.h"

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

	SetConsoleBufferResolution(512, 512);
	
	Viewport* mainViewport = new Viewport(Vector2i(0, 0), Vector2i(256, 64));

	Player* player = new Player(Vector2(22.f, 11.f), Vector2(1.f, 0.f));

	Camera* mainCam = new Camera();

	int width = mainViewport->size.x;
	int height = mainViewport->size.y;

	SetCursorVis(false);

	ToggleANSI(true);

	while (gameIsRunning)
	{
		// Timelord shenanigans
		deltaTime = (clock() - m_oldTime) / 1000.f;
		int fps = (1.f / deltaTime);
		m_oldTime = clock();
		
		float moveSpeed = deltaTime * 5.0f; // Cells per second
		float rotSpeed = deltaTime * 1.0f; // radians / second

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
			float oldPLaneX = mainCam->size.x;

			plDirX = plDirX * cos(-rotSpeed) - plDirY * sin(-rotSpeed);
			plDirY = oldDirX * sin(-rotSpeed) + plDirY * cos(-rotSpeed);

			mainCam->size.x = mainCam->size.x * cos(-rotSpeed) - mainCam->size.y * sin(-rotSpeed);
			mainCam->size.y = oldPLaneX * sin(-rotSpeed) + mainCam->size.y * cos(-rotSpeed);
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			float oldDirX = plDirX;
			float oldPLaneX = mainCam->size.x;

			plDirX = plDirX * cos(rotSpeed) - plDirY * sin(rotSpeed);
			plDirY = oldDirX * sin(rotSpeed) + plDirY * cos(rotSpeed);

			mainCam->size.x = mainCam->size.x * cos(rotSpeed) - mainCam->size.y * sin(rotSpeed);
			mainCam->size.y = oldPLaneX * sin(rotSpeed) + mainCam->size.y * cos(rotSpeed);
		}
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			gameIsRunning = false;
			break;
		}

		// Raycasting Loop

		for (int i = 0; i < width; i++)
		{
			//  Right of Screen = 1, Left of Screen = - 1
			float cameraX = 2 * i / (float)width - 1.f; // Camera X Position

			Vector2 rayDir = Vector2
			(
				plDirX + mainCam->size.x * cameraX,
				plDirY + mainCam->size.y * cameraX
			);

			// Which map cell the array is in
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

			// DDA algorithm (Digital Differential Analyser
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

			// Calculate distance of wall proected on camera direction (Direct distance to player would cause fisheye effect)
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

			/*
			// Choose Wall Color
			unsigned char color;
			switch (map->contents[mapPos.x][mapPos.y])

			{
			case 1:
			{
				color = (horizontalWall) ? CLR_RED : CLR_LIGHTRED;
				break;
			}
			case 2:
			{
				color = (horizontalWall) ? CLR_GREEN : CLR_LIGHTGREEN;
				break;
			}
			case 3:
			{
				color = (horizontalWall) ? CLR_CYAN : CLR_LIGHTCYAN;
				break;
			}
			case 4:
			{
				color = (horizontalWall) ? CLR_MAGENTA : CLR_LIGHTMAGENTA;
				break;
			}
			default:
			{
				color = (horizontalWall) ? CLR_DARKGREY : CLR_WHITE;
				break;
			}


			}

			char pixel = mainViewport->GetCharFromDepth( float( (lineHeight >= height) ? height : lineHeight ) / float(height) );

			mainViewport->AddScanlineToBuffer(i, height, drawStart, drawEnd, pixel, color, 0);
			*/

			// Choose Wall Color
			Color color;
			switch (map->contents[mapPos.x][mapPos.y])
			{
			case 1:
			{
				color = (horizontalWall) ? Color() : Color();
				break;
			}
			case 2:
			{
				color = (horizontalWall) ? Color() : Color();
				break;
			}
			case 3:
			{
				color = (horizontalWall) ? Color() : Color();
				break;
			}
			case 4:
			{
				color = (horizontalWall) ? Color() : Color();
				break;
			}
			default:
			{
				color = (horizontalWall) ? Color() : Color();
				break;
			}
			}
		}

		DrawColorViewport(mainViewport);
		// DrawASCIIViewport(mainViewport);

	}

	delete player;
	delete mainCam;
	delete mainViewport;
	delete map;

	return EXIT_SUCCESS;
}

void Game::Raycast()
{

}
