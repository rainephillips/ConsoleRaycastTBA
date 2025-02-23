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
#include "Raycast.h"

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

	SetConsoleBufferResolution(2048, 2048);
	
	Viewport* mainViewport = new Viewport(Vector2i(0, 0), Vector2i(256, 64));

	Player* player = new Player(Vector2(6.f, 7.f), Vector2(1.f, 0.f));

	Camera* mainCam = new Camera();

	Vector2i defaultTextureSize = Vector2i(64, 64);

	vector<Texture*> textureList;
	textureList.reserve(8);

	CreateDefaultTextures(textureList, Vector2i(64, 64));

	for (int i = 0; i < 8; i++)
	{
		textureList[i]->SetTexture("images\\adachitrue.jpeg");
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

		Raycaster(mainViewport, player, mainCam, map, textureList, false);

		//DrawASCIIViewport(mainViewport);
		DrawColorViewport(mainViewport);

		std::cout << "FPS: " << fps << "    ";

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

void Game::Raycaster(Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*> textures,bool useASCII)
{
	// Define References for easier code readability

	// Raycasting Loop

	for (int x = 0; x < viewport->size.x; x++)
	{
		Raycast(x, viewport, player, camera, map, textures, useASCII);
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
