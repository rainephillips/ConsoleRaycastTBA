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
	: m_oldTime{ 0.f }, m_time{ 0.f }, gameIsRunning{ true }, deltaTime{ 0.f }, m_player{ nullptr }
{

}

int Game::Run()
{
	unsigned short tempMap[24][24] =
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

	unsigned short** tempMapData = new unsigned short*[24];
	for (int y = 0; y < 24; y++)
	{
		tempMapData[y] = new unsigned short[24];
		for (int x = 0; x < 24; x++)
		{
			tempMapData[y][x] = tempMap[y][x];
		}
	}

	Map* map = new Map(24, 24);

	map->SetContents(tempMapData, Vector2i(24, 24));

	for (int y = 0; y < 24; y++)
	{
		delete[] tempMapData[y];
	}

	delete[] tempMapData;

	SetConsoleBufferResolution(1024, 1024);

	Player* m_player = new Player
	(
		Vector2(5.5f, 6.5f), Vector2(1.f, 0.f),  // Player Data
		Vector2(0.f, 0.9f), // Camera Data
		Vector2i(10, 3), Vector2i(128, 32) // Viewport Data
	);

	Camera* mainCam = m_player->GetCamera();
	
	Viewport* mainViewport = mainCam->GetViewport();

	Vector2i defaultTextureSize = Vector2i(64, 64);

	vector<Texture*> textureList;
	textureList.reserve(9);

	CreateDefaultTextures(textureList, defaultTextureSize);

	for (int i = 0; i < 8; i++)
	{
		textureList[i]->SetTexture("images\\adachitrue.jpeg");
	}

	textureList.emplace_back(new Texture("images\\adachifalse.jpeg"));
	

	int& width = mainViewport->size.x;
	int& height = mainViewport->size.y;

	SetCursorVis(false);

	ToggleANSI(true);

	/*TextureA testTexture = TextureA("images\\AlphaTestImage.png");

	Viewport* testViewport = new Viewport();

	testViewport->SetColorABuffer(testTexture.GetSize(), testTexture.GetTexture());

	DrawColorViewport(testViewport);
	
	delete testViewport;*/

	while (gameIsRunning)
	{
		// Timelord shenanigans
		deltaTime = (clock() - m_oldTime) / 1000.f;
		unsigned int fps = (1.f / deltaTime);
		m_oldTime = clock();
		

		SetConsoleCursorPos(0, (height + mainViewport->position.y));
		std::cout << "\033[2K"; // Erase current line
		std::cout << "FPS: " << fps;
		

		
		//OldKeyboardInput(player, mainCam, map);
		KeyboardInput(m_player, mainCam, map);

		m_player->RunTweens(deltaTime);

		Raycaster(mainViewport, m_player, mainCam, map, textureList, false);
		//DrawASCIIViewport(mainViewport);
		DrawColorViewport(mainViewport);
		
		

		//if (m_player->IsMoving() == false)
		//{
		//	SetCursorVis(true);

		//	string command;

		//	SetConsoleCursorPos(0, (height + mainViewport->position.y + 1));

		//	std::cout << "\033[2K"; // Erase current line

		//	std::cout << "Please enter input: ";
		//	std::getline(std::cin, command);

		//	// Reset old time not to account for time waiting typing to delta
		//	m_oldTime = clock();

		//	CommandInput(command, m_player, mainCam, map);

		//	SetCursorVis(false);
		//}

	}

	for (Texture* texture : textureList)
	{
		delete texture;
	}

	delete m_player;
	delete map;

	return EXIT_SUCCESS;
}

void Game::Raycaster(Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*> textures,bool useASCII)
{
	// Raycasting Loop
	float* zBuffer = new float[viewport->size.y];

	int* spriteOrder = new int[8]; // total number of sprites
	float* spriteDistance = new float[8] ; // total number of sprites

	if (!useASCII)
	{
		for (int y = 0; y < viewport->size.y; y++)
		{
			FloorRaycast(y, viewport, player, camera, map, textures);
		}
	}
	

	for (int x = 0; x < viewport->size.x; x++)
	{
		WallRaycast(x, viewport, player, camera, map, textures, useASCII);
	}

	delete[] zBuffer;

	delete[] spriteOrder;
	delete[] spriteDistance;
}

void Game::OldKeyboardInput(Player*& player, Camera*& camera, Map*& map)
{
	float moveSpeed = deltaTime * 5.0f; // Cells per second
	float rotSpeed = deltaTime * 3.0f; // radians / second

	float& plPosX = player->position.x;
	float& plPosY = player->position.y;

	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	unsigned short* wallData = map->GetWallData();
	Vector2i mapSize = map->GetMapSize();

	// Keyboard Inputs
	// Move Forward if not crash into wall
	if (GetAsyncKeyState(VK_UP))
	{
		if (wallData[ int(plPosY) * mapSize.x + int(plPosX + plDirX * moveSpeed) ] == 0)
		{
			plPosX += plDirX * moveSpeed;
		}
		if (wallData[ int(plPosY + plDirY * moveSpeed) * mapSize.x + int(plPosX) ] == 0)
		{
			plPosY += plDirY * moveSpeed;
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (wallData[int(plPosY) * mapSize.x + int(plPosX - plDirX * moveSpeed)] == 0)
		{
			plPosX -= plDirX * moveSpeed;
		}
		if (wallData[int(plPosY - plDirY * moveSpeed) * mapSize.x + int(plPosX)] == 0)
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
	float moveSpeed = player->GetMovementSpeed();
	float rotSpeed = player->GetRotationSpeed();

	float& plPosX = player->position.x;
	float& plPosY = player->position.y;

	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	unsigned short* wallData = map->GetWallData();
	Vector2i mapSize = map->GetMapSize();

	// Keyboard Inputs
	// Move Forward if not crash into wall
	if (GetAsyncKeyState(VK_UP))
	{
		if (player->IsMoving() == false)
		{
			if (wallData[int(plPosY) * mapSize.x + int(plPosX + plDirX)] == 0)
			{
				player->AddTween(new Tween<float>(plPosX, plPosX + plDirX, std::ref(player->position.x), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosX, plPosX + plDirX * 0.45f, std::ref(player->position.x), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosX + plDirX * 0.45, plPosX, std::ref(player->position.x), moveSpeed / 2, true));
			}

			if (wallData[int(plPosY + plDirY) * mapSize.x + int(plPosX)] == 0)
			{
				player->AddTween(new Tween<float>(plPosY, plPosY + plDirY, std::ref(player->position.y), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosY, plPosY + plDirY * 0.45f, std::ref(player->position.y), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosY + plDirY * 0.45, plPosY, std::ref(player->position.y), moveSpeed / 2, true));
			}
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		if (player->IsMoving() == false)
		{
			if (wallData[int(plPosY) * mapSize.x + int(plPosX - plDirX)] == 0)
			{
				player->AddTween(new Tween<float>(plPosX, plPosX - plDirX, std::ref(player->position.x), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosX, plPosX - plDirX * 0.45f, std::ref(player->position.x), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosX - plDirX * 0.45, plPosX, std::ref(player->position.x), moveSpeed / 2, true));
			}

			if (wallData[int(plPosY - plDirY) * mapSize.x + int(plPosX)] == 0)
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

void Game::CommandInput(string command, Player*& player, Camera*& camera, Map*& map)
{
	float rotAmt = DEG_TO_RAD(90);
	float moveSpeed = player->GetMovementSpeed();
	float rotSpeed = player->GetRotationSpeed();

	float& plPosX = player->position.x;
	float& plPosY = player->position.y;

	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	unsigned short* wallData = map->GetWallData();
	Vector2i mapSize = map->GetMapSize();

	// Convert string to lowercase
	for (int i = 0; i < command.size(); i++)
	{
		if (command[i] >= 'A' && command[i] <= 'Z')
		{
			command[i] += 32;
		}
	}


	if (command == "move forward")
	{
		if (player->IsMoving() == false)
		{
			if (wallData[int(plPosY) * mapSize.x + int(plPosX + plDirX)] == 0)
			{
				player->AddTween(new Tween<float>(plPosX, plPosX + plDirX, std::ref(player->position.x), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosX, plPosX + plDirX * 0.45f, std::ref(player->position.x), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosX + plDirX * 0.45, plPosX, std::ref(player->position.x), moveSpeed / 2, true));
			}

			if (wallData[int(plPosY + plDirY) * mapSize.x + int(plPosX)] == 0)
			{
				player->AddTween(new Tween<float>(plPosY, plPosY + plDirY, std::ref(player->position.y), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosY, plPosY + plDirY * 0.45f, std::ref(player->position.y), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosY + plDirY * 0.45, plPosY, std::ref(player->position.y), moveSpeed / 2, true));
			}
		}
	}
	else if (command == "move backward")
	{
		if (player->IsMoving() == false)
		{
			if (wallData[int(plPosY) * mapSize.x + int(plPosX - plDirX)] == 0)
			{
				player->AddTween(new Tween<float>(plPosX, plPosX - plDirX, std::ref(player->position.x), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosX, plPosX - plDirX * 0.45f, std::ref(player->position.x), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosX - plDirX * 0.45, plPosX, std::ref(player->position.x), moveSpeed / 2, true));
			}

			if (wallData[int(plPosY - plDirY) * mapSize.x + int(plPosX)] == 0)
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
	else if (command == "turn left")
	{
		float plNewDirX = plDirX * cos(-rotAmt) - plDirY * sin(-rotAmt);
		float plNewDirY = plDirX * sin(-rotAmt) + plDirY * cos(-rotAmt);

		float newCamSizeX = camera->size.x * cos(-rotAmt) - camera->size.y * sin(-rotAmt);
		float newCamSizeY = camera->size.x * sin(-rotAmt) + camera->size.y * cos(-rotAmt);

		if (player->IsMoving() == false)
		{
			// Rotate Player
			player->AddTween(new Tween<float>(plDirX, plNewDirX, std::ref(plDirX), rotSpeed, true));
			player->AddTween(new Tween<float>(plDirY, plNewDirY, std::ref(plDirY), rotSpeed, true));

			// Rotate Camera
			player->AddTween(new Tween<float>(camera->size.x, newCamSizeX, std::ref(camera->size.x), rotSpeed, true));
			player->AddTween(new Tween<float>(camera->size.y, newCamSizeY, std::ref(camera->size.y), rotSpeed, true));
		}
	}
	else if (command == "turn right")
	{
		float plNewDirX = plDirX * cos(rotAmt) - plDirY * sin(rotAmt);
		float plNewDirY = plDirX * sin(rotAmt) + plDirY * cos(rotAmt);

		float newCamSizeX = camera->size.x * cos(rotAmt) - camera->size.y * sin(rotAmt);;
		float newCamSizeY = camera->size.x * sin(rotAmt) + camera->size.y * cos(rotAmt);;

		if (player->IsMoving() == false)
		{
			// Rotate Player
			player->AddTween(new Tween<float>(plDirX, plNewDirX, std::ref(plDirX), rotSpeed, true));
			player->AddTween(new Tween<float>(plDirY, plNewDirY, std::ref(plDirY), rotSpeed, true));

			// Rotate Camera
			player->AddTween(new Tween<float>(camera->size.x, newCamSizeX, std::ref(camera->size.x), rotSpeed, true));
			player->AddTween(new Tween<float>(camera->size.y, newCamSizeY, std::ref(camera->size.y), rotSpeed, true));
		}
	}
	else if (command == "escape")
	{
		gameIsRunning = false;
		return;
	}
	else
	{

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
