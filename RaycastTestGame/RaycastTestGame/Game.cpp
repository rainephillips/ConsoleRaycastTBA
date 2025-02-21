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
	  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
	  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
	  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
	  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
	  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
	  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
	  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
	  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
	  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
	  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
	  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
	  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
	  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
	  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
	  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
	  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
	  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
	  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
	  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
	  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
	  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
	  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
	  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
	  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
	};

	Map* map = new Map(24, 24);
	map->SetContents(tempMap, Vector2i(24, 24));

	SetConsoleBufferResolution(2048, 512);
	
	Viewport* mainViewport = new Viewport(Vector2i(10, 6), Vector2i(240, 64));

	Player* player = new Player(Vector2(6.f, 7.f), Vector2(1.f, 0.f));

	Camera* mainCam = new Camera();

	Vector2i defaultTextureSize = Vector2i(64, 64);

	vector<Texture*> textureList;
	textureList.reserve(8);

	CreateDefaultTextures(textureList, Vector2i(64, 64));

	for (int i = 0; i < 8; i++)
	{
		textureList[i]->SetTexture("images\\akechi.jpeg");
	}
	

	int& width = mainViewport->size.x;
	int& height = mainViewport->size.y;

	SetCursorVis(false);

	ToggleANSI(true);

	/*Viewport* testViewport = new Viewport();

	testViewport->SetColorBuffer(textureList[0]->GetSize(), textureList[1]->GetTexture());

	DrawColorViewport(testViewport);

	delete testViewport;*/

	while (gameIsRunning)
	{
		// Timelord shenanigans
		deltaTime = (clock() - m_oldTime) / 1000.f;
		int fps = (1.f / deltaTime);
		m_oldTime = clock();

		
		GetAsyncKeyboardInput(player, mainCam, map);

		Raycast(mainViewport, player, mainCam, map, textureList, false);

		DrawColorViewport(mainViewport);
		//DrawASCIIViewport(mainViewport);

	}

	for (Texture* texture : textureList)
	{
		delete texture;
	}

	delete player;
	delete mainCam;
	delete mainViewport;
	delete map;

	return EXIT_SUCCESS;
}

void Game::Raycast(Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*> textures,bool useASCII)
{
	// Define References for easier code readability

	float& plPosX = player->position.x;
	float& plPosY = player->position.y;

	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	int& width = viewport->size.x;
	int& height = viewport->size.y;

	// Raycasting Loop

	for (int x = 0; x < width; x++)
	{
		//  Right of Screen = 1, Left of Screen = - 1
		float cameraX = 2 * x / (float)width - 1.f; // Camera X Position

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
		bool isHorizontalWall; // If a NS or EW wall was hit

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
				isHorizontalWall = false;
			}
			else
			{
				sideDist.y += deltaDist.y;
				mapPos.y += step.y;
				isHorizontalWall = true;
			}

			// Check if ray has hit a wall
			if (map->contents[mapPos.x][mapPos.y] > 0)
			{
				wallHit = true;
			}
		}

		// Calculate distance of wall projected on camera direction (Direct distance to player would cause fisheye effect)
		if (!isHorizontalWall)
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
			unsigned char color = GetASCIIColorFromRaycast(mapPos.x, mapPos.y, map, isHorizontalWall);

			// Get Character
			char pixel = viewport->GetCharFromDepth(float((lineHeight >= height) ? height : lineHeight) / float(height));

			//Add Line to Buffer
			viewport->AddScanlineToBuffer(x, height, drawStart, drawEnd, pixel, color, 0);
		}
		else
		{
			// Allows texture 0 to be used
			int texNum = map->contents[mapPos.x][mapPos.y] - 1;

			// Calculate exact part of the wall hit instead of just cell
			float wallX; // Technically its the y cord of the wall if its 
			// horizontal but its the x cord of the texture
			if (isHorizontalWall)
			{
				wallX = plPosX + perpWallDist * rayDir.x;
			}
			else
			{
				wallX = plPosY + perpWallDist * rayDir.y;
			}
			wallX -= floor(wallX); // % of the x coordinate if start = 0 and end = 1

			Vector2i texSize = textures[texNum]->GetSize();

			// Get X Coordinate on the texture

			// Flip Texture Accordingly Depending on if on NSEW Wall
			int texPosX = int(wallX * (float)texSize.x);
			if (isHorizontalWall == 0 && rayDir.x < 0)
			{
				texPosX = texSize.x - texPosX - 1;
			}
			if (isHorizontalWall == 1 && rayDir.y > 0)
			{
				texPosX = texSize.x - texPosX - 1;
			}
			// X coordinate stays the same for each cast but y has to be calculated for each

			// How much to increase the texture coordinate per pixel
			float step = 1.f * texSize.y / lineHeight;

			// Starting texture coordinate
			float texPosY = (drawStart - (height / 2.f) + (lineHeight / 2)) * step;

			// Repeat for each character in the raycast
			Color color;
			for (int y = 0; y < height; y++)
			{
				
				if (y > drawStart && y <= drawEnd)
				{
					// Cast the texture coordinate to integer, and bitwise and with (textHeight - 1) for overflow
					int texY = (int)texPosY & (texSize.y - 1);
					

					color = textures[texNum]->GetColorFromLocation(texPosX, texPosY);
					texPosY += step;
				}
				else
				{
					color = (y <= drawStart) ? Color(60, 73, 82) : Color(112, 86, 47);
				}
				
				viewport->AddColorToBuffer(x, y, color);
			}

			//// Get Wall Color
			//Color color = GetColorFromRaycast(mapPos.x, mapPos.y, map, isHorizontalWall);

			//// Add Color to buffer
			//viewport->AddScanlineToColorBuffer(i, height, drawStart, drawEnd, color);
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

void Game::CreateDefaultTextures(std::vector<Texture*>& textureList, Vector2i textureSize)
{
	for (int i = 0; i < 8; i++)
	{
		Texture* texture = new Texture();
		texture->CreateNewTexture(textureSize);
		textureList.emplace_back(texture);
	}
	for (int x = 0; x < textureSize.x; x++)
	{
		for (int y = 0; y < textureSize.y; y++)
		{
			int xColor = (x * 256 / textureSize.x);
			int yColor = (y * 256 / textureSize.y);

			int xyColor = (y * 128 / textureSize.y + x * 128 / textureSize.x);
			int xorColor = xColor ^ yColor;
			textureList[0]->SetTextureColor(x, y, Color( 255 * (x != y && x != textureSize.x - y) ) ); // flat red texture w/ black cross
			textureList[1]->SetTextureColor(x, y, Color( xyColor + 245 * xyColor + 65536 * xyColor )); // sloped greyscale
			textureList[2]->SetTextureColor(x, y, Color( 256 * xyColor + 65536 * xyColor)); // sloped yellow gradient
			textureList[3]->SetTextureColor(x, y, Color( xorColor + 256 * xorColor + 65536 * xorColor )); // xor greyscale
			textureList[4]->SetTextureColor(x, y, Color( 256 * xorColor) ); // xor green
			textureList[5]->SetTextureColor(x, y, Color( 255 * (x % 16 && y % 16) ) ); // red bricks
			textureList[6]->SetTextureColor(x, y, Color( 255 * yColor) ); // red gradient
			textureList[7]->SetTextureColor(x, y, Color( 8421504 )); // flat gray texture
		}
	}
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
