#include "Game.h"

#include <iostream>
#include <cmath>
#include <ctime>
#include <conio.h>

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

	SetConsoleBufferResolution(128, 128);
	
	Viewport* mainViewport = new Viewport(Vector2i(0, 0), Vector2i(200, 60));

	Player* player = new Player(Vector2(22.f, 12.f), Vector2(-1.f, 0.f));

	Camera* mainCam = new Camera();

	int width = mainViewport->size.x;
	int height = mainViewport->size.y;

	while (gameIsRunning)
	{
		// Timelord shenanigans
		deltaTime = (clock() - m_oldTime) / 1000.f;
		int fps = (1.f / deltaTime);
		m_oldTime = clock();
		
		float moveSpeed = deltaTime * 5.0f; // Cells per second
		float rotSpeed = deltaTime * 3.0f; // radians / second

		// Keyboard Inputs

		// Raycasting Loop
		for (int i = 0; i < width; i++)
		{
			//  Right of Screen = 1, Left of Screen = - 1
			float cameraX = 2 * i / static_cast<float>(width) - 1.f; // Camera X Position

			Vector2 rayDir = Vector2
			(
				player->rotation.x + mainCam->size.x * cameraX,
				player->rotation.y + mainCam->size.x * cameraX
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
					sideDist.x += deltaDist.y;
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

			// Calculat height of line to draw on screen
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
					color = (horizontalWall) ? CLR_LIGHTGRAY : CLR_WHITE;
					break;
				}

				
			}

			DrawVertLine(i, drawStart, drawEnd, '#', color, 0);
		}

		player->rotation.x += 0.01f;


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
