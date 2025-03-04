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
#include "Sprite.h"

/* TO DO
NEEDED:
	ADD SPRITES
	ADD SETTINGS AS BITWISE VALUE

ASSESSMENT COMPLIANCE:
	NEW COMMANDS FOR ASSESMENT COMPLIANCE
	ADD ITEMS TO ROOMS
	ADD ROOM DESCIPTION
	ADD ITEMS
	ADD SPELLS

OPTIONAL:
	ADD MAP EDITOR / CREATOR
*/

using std::vector;

Game::Game()
	: m_oldTime{ 0.f }, m_time{ 0.f }, gameIsRunning{ true }, deltaTime{ 0.f }, m_player{ nullptr }
{

}

int Game::Run()
{
	// Create Map
	uint16_t tempMapWall[24 * 24] =
	{
	  8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4,
	  8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4,
	  8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6,
	  8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,
	  8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4,
	  8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6,
	  8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6,
	  7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6,
	  7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6,
	  7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4,
	  7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6,
	  7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6,
	  7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3,
	  2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3,
	  2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3,
	  2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3,
	  1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3,
	  2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5,
	  2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5,
	  2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5,
	  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,
	  2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5,
	  2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5,
	  2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5
	};

	uint16_t* tempMapData = new uint16_t[24 * 24];
	for (int i = 0; i < 24 * 24; i++)
	{
		tempMapData[i] = tempMapWall[i];
	}

	Map* m_currentMap = new Map(24, 24);

	m_currentMap->SetContentDataType(tempMapData, MapDataType::WALL, Vector2i(24, 24));

	delete[] tempMapData;

	// Create Player Camera & Viewport

	Player* m_player = new Player
	(
		Vector2(22.5f, 1.5f), Vector2(1.f, 0.f),  // Player Data
		Vector2(0.f, 1.4f), // Camera Data
		Vector2i(10, 3), Vector2i(128, 32) // Viewport Data
	);

	Camera* mainCam = m_player->GetCamera();
	
	Viewport* mainViewport = mainCam->GetViewport();

	int& width = mainViewport->size.x;
	int& height = mainViewport->size.y;

	// Create Textures

	Vector2i defaultTextureSize = Vector2i(64, 64);

	m_textureList.reserve(9);

	CreateDefaultTextures(m_textureList, defaultTextureSize);

	//for (int i = 0; i < 8; i++)
	//{
	//	m_textureList[i]->SetTexture("images\\trollface.png");
	//}

	m_textureList.emplace_back(new Texture("images\\adachifalse.jpeg"));

	// Add sprite textures

	m_textureList.emplace_back(new Texture("images\\smalladachi.png"));
	m_textureList.emplace_back(new Texture("images\\coffeecup.png"));
	m_textureList.emplace_back(new Texture("images\\lobotomy.jpeg"));

	//Texture testTexture = Texture("images\\trollface.png");

	//Viewport* testViewport = new Viewport();

	//testViewport->SetColorABuffer(testTexture.GetSize(), testTexture.GetTexture());

	//DrawColorViewport(testViewport);

	//delete testViewport;

	// Add Sprites to map TEMP

	m_currentMap->AddSprite(new Sprite( Vector2(20.5f, 11.5f), m_textureList[11]) );
	m_currentMap->AddSprite(new Sprite( Vector2(18.5f, 4.5f), m_textureList[11]) );
	m_currentMap->AddSprite(new Sprite( Vector2(10.5f, 4.5f), m_textureList[11]) );
	m_currentMap->AddSprite(new Sprite( Vector2(10.5f, 12.5f), m_textureList[11]) );
	m_currentMap->AddSprite(new Sprite( Vector2(3.5f, 6.5f), m_textureList[11]) );
	m_currentMap->AddSprite(new Sprite( Vector2(3.5f, 20.5f), m_textureList[11]) );
	m_currentMap->AddSprite(new Sprite( Vector2(3.5f, 14.5f), m_textureList[11]) );
	m_currentMap->AddSprite(new Sprite( Vector2(14.5f, 20.5f), m_textureList[11]) );

	m_currentMap->AddSprite(new Sprite(Vector2(18.5f, 10.5f), m_textureList[10]));
	m_currentMap->AddSprite(new Sprite(Vector2(18.5f, 11.5f), m_textureList[10]));
	m_currentMap->AddSprite(new Sprite(Vector2(18.5f, 12.5f), m_textureList[10]));

	m_currentMap->AddSprite(new Sprite(Vector2(21.5f, 1.5f), m_textureList[9]));
	m_currentMap->AddSprite(new Sprite(Vector2(15.5f, 1.5f), m_textureList[9]));
	m_currentMap->AddSprite(new Sprite(Vector2(16.0f, 1.8f), m_textureList[9]));
	m_currentMap->AddSprite(new Sprite(Vector2(16.2f, 1.2f), m_textureList[9]));
	m_currentMap->AddSprite(new Sprite(Vector2(3.5f, 2.5f), m_textureList[9]));
	m_currentMap->AddSprite(new Sprite(Vector2(9.5f, 15.5f), m_textureList[9]));
	m_currentMap->AddSprite(new Sprite(Vector2(10.0f, 15.1f), m_textureList[9]));
	m_currentMap->AddSprite(new Sprite(Vector2(10.5f, 15.8f), m_textureList[9]));

	// Console Settings

	SetConsoleBufferResolution(1024, 1024);

	SetCursorVis(false);

	ToggleANSI(true);


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
		KeyboardInput(m_player, mainCam, m_currentMap);

		m_player->RunTweens(deltaTime);

		Raycaster(mainViewport, m_player, mainCam, m_currentMap, m_textureList, false);
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

		//	CommandInput(command, m_player, mainCam, m_currentMap);

		//	SetCursorVis(false);
		//}

	}

	for (Texture* texture : m_textureList)
	{
		delete texture;
	}

	delete m_player;
	delete m_currentMap;

	return EXIT_SUCCESS;
}

void Game::Raycaster(Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*> textures, bool useASCII)
{
	// Raycasting Loop
	float* zBuffer = new float[viewport->size.x];

	if (!useASCII)
	{
		for (int y = 0; y < viewport->size.y; y++)
		{
			FloorRaycast(y, viewport, player, camera, map, textures);
		}
	}
	

	for (int x = 0; x < viewport->size.x; x++)
	{
		WallRaycast(x, viewport, player, camera, map, textures, useASCII, zBuffer);
	}

	SpriteCasting(viewport, player, camera, textures, map, zBuffer);

	delete[] zBuffer;
}

void Game::OldKeyboardInput(Player*& player, Camera*& camera, Map*& map)
{
	float moveSpeed = deltaTime * 5.0f; // Cells per second
	float rotSpeed = deltaTime * 3.0f; // radians / second

	float& plPosX = player->position.x;
	float& plPosY = player->position.y;

	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	uint16_t* wallData = map->GetDataTypeBuffer(MapDataType::WALL);
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
	}

	delete[] wallData;
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

	uint16_t* wallData = map->GetDataTypeBuffer(MapDataType::WALL);
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
				player->AddTween(new Tween<float>(plPosX + plDirX * 0.45f, plPosX, std::ref(player->position.x), moveSpeed / 2, true));
			}

			if (wallData[int(plPosY + plDirY) * mapSize.x + int(plPosX)] == 0)
			{
				player->AddTween(new Tween<float>(plPosY, plPosY + plDirY, std::ref(player->position.y), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosY, plPosY + plDirY * 0.45f, std::ref(player->position.y), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosY + plDirY * 0.45f, plPosY, std::ref(player->position.y), moveSpeed / 2, true));
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
				player->AddTween(new Tween<float>(plPosX - plDirX * 0.45f, plPosX, std::ref(player->position.x), moveSpeed / 2, true));
			}

			if (wallData[int(plPosY - plDirY) * mapSize.x + int(plPosX)] == 0)
			{
				player->AddTween(new Tween<float>(plPosY, plPosY - plDirY, std::ref(player->position.y), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosY, plPosY - plDirY * 0.45f, std::ref(player->position.y), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosY - plDirY * 0.45f, plPosY, std::ref(player->position.y), moveSpeed / 2, true));
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

	delete[] wallData;
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

	uint16_t* wallData = map->GetDataTypeBuffer(MapDataType::WALL);
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
				player->AddTween(new Tween<float>(plPosX + plDirX * 0.45f, plPosX, std::ref(player->position.x), moveSpeed / 2, true));
			}

			if (wallData[int(plPosY + plDirY) * mapSize.x + int(plPosX)] == 0)
			{
				player->AddTween(new Tween<float>(plPosY, plPosY + plDirY, std::ref(player->position.y), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosY, plPosY + plDirY * 0.45f, std::ref(player->position.y), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosY + plDirY * 0.45f, plPosY, std::ref(player->position.y), moveSpeed / 2, true));
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
				player->AddTween(new Tween<float>(plPosX - plDirX * 0.45f, plPosX, std::ref(player->position.x), moveSpeed / 2, true));
			}

			if (wallData[int(plPosY - plDirY) * mapSize.x + int(plPosX)] == 0)
			{
				player->AddTween(new Tween<float>(plPosY, plPosY - plDirY, std::ref(player->position.y), moveSpeed, true));
			}
			else
			{
				player->AddTween(new Tween<float>(plPosY, plPosY - plDirY * 0.45f, std::ref(player->position.y), moveSpeed / 2, false));
				player->AddTween(new Tween<float>(plPosY - plDirY * 0.45f, plPosY, std::ref(player->position.y), moveSpeed / 2, true));
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
	}
	else
	{

	}

	delete[] wallData;
}

void Game::CreateDefaultTextures(vector<Texture*>& textureList, Vector2i textureSize)
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
			textureList[0]->SetTextureColor(x, y, ColorA( 255 * (x != y && x != textureSize.x - y) ) ); // flat red texture w/ black cross
			textureList[1]->SetTextureColor(x, y, ColorA( xyColor + 245 * xyColor + 65536 * xyColor )); // sloped greyscale
			textureList[2]->SetTextureColor(x, y, ColorA( 256 * xyColor + 65536 * xyColor)); // sloped yellow gradient
			textureList[3]->SetTextureColor(x, y, ColorA( xorColor + 256 * xorColor + 65536 * xorColor )); // xor greyscale
			textureList[4]->SetTextureColor(x, y, ColorA( 256 * xorColor) ); // xor green
			textureList[5]->SetTextureColor(x, y, ColorA( 255 * (x % 16 && y % 16) ) ); // red bricks
			textureList[6]->SetTextureColor(x, y, ColorA( 255 * yColor) ); // red gradient
			textureList[7]->SetTextureColor(x, y, ColorA( 8421504 )); // flat gray texture
		}
	}
}
