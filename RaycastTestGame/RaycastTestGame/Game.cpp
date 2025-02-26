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

	SetConsoleBufferResolution(1024, 1024);
	
	Viewport* mainViewport = new Viewport(Vector2i(10, 4), Vector2i(128, 32));

	Player* player = new Player(Vector2(5.5f, 6.5f), Vector2(1.f, 0.f));

	Camera* mainCam = new Camera();

	Vector2i defaultTextureSize = Vector2i(64, 64);

	vector<Texture*> textureList;
	textureList.reserve(9);

	CreateDefaultTextures(textureList, Vector2i(64, 64));

	for (int i = 0; i < 8; i++)
	{
		textureList[i]->SetTexture("images\\adachitrue.jpeg");
	}

	textureList.emplace_back(new Texture());
	textureList[8]->SetTexture("images\\adachifalse.jpeg");
	

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

		
		//OldKeyboardInput(player, mainCam, map);
		KeyboardInput(player, mainCam, map);

		player->RunTweens(deltaTime);

		if (player->IsMoving())
		{
			Raycaster(mainViewport, player, mainCam, map, textureList, false);
			//DrawASCIIViewport(mainViewport);
			DrawColorViewport(mainViewport);
		}
		

		

		//std::cout << "FPS: " << fps << "    ";

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

	for (int y = 0; y < viewport->size.y; y++)
	{
		FloorRaycast(y, viewport, player, camera, map, textures);
	}

	for (int x = 0; x < viewport->size.x; x++)
	{
		WallRaycast(x, viewport, player, camera, map, textures, useASCII);
	}
}

void Game::OldKeyboardInput(Player*& player, Camera*& camera, Map*& map)
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
		float plNewDirX = plDirX * cos(-rotSpeed) - plDirY * sin(-rotSpeed);
		float plNewDirY = plDirX * sin(-rotSpeed) + plDirY * cos(-rotSpeed);

		float newCamSizeX = camera->size.x * cos(-rotSpeed) - camera->size.y * sin(-rotSpeed);
		float newCamSizeY = camera->size.x * sin(-rotSpeed) + camera->size.y * cos(-rotSpeed);

		plDirX = plNewDirX;
		plDirY = plNewDirY;

		camera->size.x = newCamSizeX;
		camera->size.y = newCamSizeY;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		float plNewDirX = plDirX * cos(rotSpeed) - plDirY * sin(rotSpeed);
		float plNewDirY = plDirX * sin(rotSpeed) + plDirY * cos(rotSpeed);

		float newCamSizeX = camera->size.x * cos(rotSpeed) - camera->size.y * sin(rotSpeed);;
		float newCamSizeY = camera->size.x * sin(rotSpeed) + camera->size.y * cos(rotSpeed);;

		plDirX = plNewDirX;
		plDirY = plNewDirY;

		camera->size.x = newCamSizeX;
		camera->size.y = newCamSizeY;

	}
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		gameIsRunning = false;
		return;
	}
}

void Game::KeyboardInput(Player*& player, Camera*& camera, Map*& map)
{
	float rotAmt = DEG_TO_RAD(90);
	float moveSpeed = 0.25f;
	float rotSpeed = .33f;

	float& plPosX = player->position.x;
	float& plPosY = player->position.y;

	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	// Keyboard Inputs
	// Move Forward if not crash into wall
	if (GetAsyncKeyState(VK_UP))
	{
		if (player->IsMoving() == false)
		{
			if (map->contents[int(plPosX + plDirX)][int(plPosY)] == 0)
			{
				player->AddTween(new Tween<float>(plPosX, plPosX + plDirX, std::ref(player->position.x), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosX, plPosX + plDirX * 0.45f, std::ref(player->position.x), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosX + plDirX * 0.45, plPosX, std::ref(player->position.x), moveSpeed / 2, true));
			}

			if (map->contents[int(plPosX)][int(plPosY + plDirY)] == 0)
			{
				player->AddTween(new Tween<float>(plPosY, plPosY + plDirY, std::ref(player->position.y), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosY, plPosY + plDirY * 0.45f, std::ref(player->position.y), moveSpeed, false));
				player->AddTween(new Tween<float>(plPosY + plDirY * 0.45, plPosY, std::ref(player->position.y), moveSpeed, true));
			}
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (player->IsMoving() == false)
		{
			if (map->contents[int(plPosX - plDirX)][int(plPosY)] == 0)
			{
				player->AddTween(new Tween<float>(plPosX, plPosX - plDirX, std::ref(player->position.x), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosX, plPosX - plDirX * 0.45f, std::ref(player->position.x), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosX - plDirX * 0.45, plPosX, std::ref(player->position.x), moveSpeed / 2, true));
			}

			if (map->contents[int(plPosX)][int(plPosY - plDirY)] == 0)
			{
				player->AddTween(new Tween<float>(plPosY, plPosY - plDirY, std::ref(player->position.y), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosY, plPosY - plDirY * 0.45f, std::ref(player->position.y), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosY - plDirY * 0.45, plPosY, std::ref(player->position.y), moveSpeed / 2, true));
			}
		}

	}
	if (GetAsyncKeyState(VK_LEFT))
	{

		float plNewDirX = plDirX * cos(-rotAmt) - plDirY * sin(-rotAmt);
		float plNewDirY = plDirX * sin(-rotAmt) + plDirY * cos(-rotAmt);

		float newCamSizeX = camera->size.x * cos(-rotAmt) - camera->size.y * sin(-rotAmt);
		float newCamSizeY = camera->size.x * sin(-rotAmt) + camera->size.y * cos(-rotAmt);

		if (player->IsMoving() == false)
		{
			player->AddTween(new Tween<float>(plDirX, plNewDirX, std::ref(plDirX), rotSpeed, true));
			player->AddTween(new Tween<float>(plDirY, plNewDirY, std::ref(plDirY), rotSpeed, true));
			player->AddTween(new Tween<float>(camera->size.x, newCamSizeX, std::ref(camera->size.x), rotSpeed, true));
			player->AddTween(new Tween<float>(camera->size.y, newCamSizeY, std::ref(camera->size.y), rotSpeed, true));
		}
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{

		float plNewDirX = plDirX * cos(rotAmt) - plDirY * sin(rotAmt);
		float plNewDirY = plDirX * sin(rotAmt) + plDirY * cos(rotAmt);

		float newCamSizeX = camera->size.x * cos(rotAmt) - camera->size.y * sin(rotAmt);;
		float newCamSizeY = camera->size.x * sin(rotAmt) + camera->size.y * cos(rotAmt);;

		if (player->IsMoving() == false)
		{
			player->AddTween(new Tween<float>(plDirX, plNewDirX, std::ref(plDirX), rotSpeed, true));
			player->AddTween(new Tween<float>(plDirY, plNewDirY, std::ref(plDirY), rotSpeed, true));
			player->AddTween(new Tween<float>(camera->size.x, newCamSizeX, std::ref(camera->size.x), rotSpeed, true));
			player->AddTween(new Tween<float>(camera->size.y, newCamSizeY, std::ref(camera->size.y), rotSpeed, true));
		}
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
