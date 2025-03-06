#include "Game.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <format>
#include <string>
#include <vector>

#include "ConsoleUtils.h"
#include "Map.h"
#include "Player.h"
#include "Raycast.h"
#include "Room.h"
#include "Sprite.h"
#include "Texture.h"
#include "Viewport.h"

/* TO DO
NEEDED:
	

ASSESSMENT COMPLIANCE:
	NEW COMMANDS FOR ASSESMENT COMPLIANCE
	ADD ITEMS TO ROOMS
	ADD SPELLS

OPTIONAL:
	ADD MAP EDITOR / CREATOR
	ADD SETTINGS AS BITWISE VALUE
*/

using std::vector;

Game::Game()
	: m_oldTime{ 0.f }, m_time{ 0.f }, m_deltaTime{ 0.f }, 
	m_gameIsRunning{ true },  m_player{ nullptr }, m_currentMap{ nullptr },
	m_currentRoom{ 0, 0 }, m_rooms{ nullptr }
{

}

int Game::Run()
{
	int exitCode;
	
	exitCode = BeginPlay();

	if (exitCode != 0)
	{
		return exitCode;
	}

	while (m_gameIsRunning)
	{
		// Timelord shenanigans
		m_deltaTime = (clock() - m_oldTime) / 1000.f;
		m_oldTime = clock();

		exitCode = Tick(m_deltaTime);

		if (exitCode != 0)
		{
			return exitCode;
		}

	}

	exitCode = EndPlay();

	return exitCode;
	
}

int Game::BeginPlay()
{
	// Create Textures

	Vector2i defaultTextureSize = Vector2i(64, 64);

	m_textureList.reserve(9);

	CreateDefaultTextures(m_textureList, defaultTextureSize);

	//for (int i = 0; i < 8; i++)
	//{
	//	m_textureList[i]->SetTexture("images\\coffeecup.png");
	//}

	m_textureList.emplace_back(new Texture("images\\adachifalse.jpeg"));

	// Add sprite textures

	m_textureList.emplace_back(new Texture("images\\coffeecup.png"));
	m_textureList.emplace_back(new Texture("images\\lobotomy.jpeg"));
	m_textureList.emplace_back(new Texture("images\\smalladachi.png"));

	//Texture testTexture = Texture("images\\smalladachi.png");

	//Viewport* testViewport = new Viewport();

	//testViewport->SetColorABuffer(testTexture.GetSize(), testTexture.GetTexture());

	//DrawColorViewport(testViewport);

	//delete testViewport;

	// Create Rooms
	m_roomsSize = Vector2i(2, 2);

	m_rooms = new Room** [m_roomsSize.y];

	for (int y = 0; y < m_roomsSize.y; y++)
	{
		m_rooms[y] = new Room*[m_roomsSize.x];
		for (int x = 0; x < m_roomsSize.x; x++)
		{
			m_rooms[y][x] = nullptr;
		}
	}

	Room* room1 = new Room();

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

	room1->SetMap(new Map(24, 24));

	room1->GetMap()->SetContentDataType(tempMapData, MapDataType::WALL, Vector2i(24, 24));

	delete[] tempMapData;

	room1->SetStartingPosition(Vector2(22.5f, 1.5f));
	room1->SetStartingDirection(Vector2(-1.f, 0.f));

	// Add Sprites to map TEMP

	Sprite* sprite;

	sprite = new Sprite(Vector2(20.5f, 11.5f), m_textureList[11]);
	sprite->SetYOffset(-40.f);
	sprite->SetScale(Vector2(2, 2));

	room1->GetMap()->AddSprite(sprite);
	room1->GetMap()->AddSprite(new Sprite(Vector2(18.5f, 4.5f), m_textureList[11]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(10.5f, 4.5f), m_textureList[11]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(10.5f, 12.5f), m_textureList[11]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(3.5f, 6.5f), m_textureList[11]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(3.5f, 20.5f), m_textureList[11]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(3.5f, 14.5f), m_textureList[11]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(14.5f, 20.5f), m_textureList[11]));

	room1->GetMap()->AddSprite(new Sprite(Vector2(18.5f, 10.5f), m_textureList[10]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(18.5f, 11.5f), m_textureList[10]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(18.5f, 12.5f), m_textureList[10]));

	room1->GetMap()->AddSprite(new Sprite(Vector2(21.5f, 1.5f), m_textureList[9]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(15.5f, 1.5f), m_textureList[9]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(16.0f, 1.8f), m_textureList[9]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(16.2f, 1.2f), m_textureList[9]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(3.5f, 2.5f), m_textureList[9]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(9.5f, 15.5f), m_textureList[9]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(10.0f, 15.1f), m_textureList[9]));
	room1->GetMap()->AddSprite(new Sprite(Vector2(10.5f, 15.8f), m_textureList[9]));

	SetRoom(Vector2i(0, 0), room1);

	// Create Player Camera & Viewport

	m_player = new Player
	(
		Vector2(22.5f, 1.5f), Vector2(-1.f, 0.f),  // Player Data
		Vector2(0.f, -1.4f), // Camera Data
		Vector2i(10, 3), Vector2i(128, 32) // Viewport Data
	);


	// Console Settings

	SetConsoleBufferResolution(1024, 1024);

	SetCursorVis(false);

	ToggleANSI(true);

	// Set Beginner Room

	ChangeRoom(GetRoom(Vector2i(0, 0)));

	return 0;
}

int Game::EndPlay()
{
	for (int i = 0; i < m_textureList.size(); i++)
	{
		if (m_textureList[i] != nullptr)
		{
			delete m_textureList[i];
		}
	}

	if (m_rooms != nullptr)
	{
		for (int y = 0; y < m_roomsSize.y; y++)
		{
			for (int x = 0; x < m_roomsSize.x; x++)
			{
				if (m_rooms[y][x] != nullptr)
				{
					delete m_rooms[y][x];
				}
			}
			delete[] m_rooms[y];
		}
	}
	
	delete[] m_rooms;

	m_textureList.clear();

	delete m_player;

	return EXIT_SUCCESS;
}

int Game::Tick(float deltaTime)
{
	Camera* mainCam = m_player->GetCamera();

	Viewport* mainViewport = mainCam->GetViewport();

	int& width = mainViewport->size.x;
	int& height = mainViewport->size.y;

	unsigned int fps = (1.f / deltaTime);

	SetConsoleCursorPos(0, (height + mainViewport->position.y));
	std::cout << "\033[2K"; // Erase current line
	std::cout << "FPS: " << fps;

	SetConsoleCursorPos(0, (height + mainViewport->position.y + 1));
	std::cout << "\033[2K"; // Erase current line
	std::cout << std::format("Player Position: [{}, {}]", int(m_player->position.x), int(m_player->position.y));

	SetConsoleCursorPos(0, (height + mainViewport->position.y + 2));
	std::cout << "\033[2K"; // Erase current line
	std::cout << std::format("Player Direction: [{}, {}]", int(m_player->direction.x), int(m_player->direction.y));

	m_player->RunTweens(deltaTime);

	//OldKeyboardInput(m_player, mainCam, m_currentMap, m_deltaTime);
	//KeyboardInput(m_player, mainCam, m_currentMap);

	//mainViewport->ClearViewport(true);

	Raycaster(mainViewport, m_player, mainCam, m_currentMap, m_textureList, false);
	//DrawASCIIViewport(mainViewport);
	DrawColorViewport(mainViewport);



	if (m_player->IsMoving() == false)
	{
		SetCursorVis(true);

		string command;

		SetConsoleCursorPos(0, (height + mainViewport->position.y + 1));

		std::cout << "\033[2K"; // Erase current line

		std::cout << "Please enter input: ";
		std::getline(std::cin, command);
		std::cin.clear();

		// Reset old time not to account for time waiting typing to delta
		m_oldTime = clock();

		CommandInput(command, m_player, mainCam, m_currentMap);

		SetCursorVis(false);
	}

	return 0;
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

void Game::OldKeyboardInput(Player*& player, Camera*& camera, Map*& map, float deltaTime)
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
		m_gameIsRunning = false;
	}

	delete[] wallData;
}

void Game::KeyboardInput(Player*& player, Camera*& camera, Map*& map)
{
	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	// Keyboard Inputs
	// Move Forward if not crash into wall
	if (player->IsMoving() == false)
	{
		if (GetAsyncKeyState(VK_UP))
		{
			player->PlayerMoveAttempt(Vector2(plDirX, plDirY), map);
			return;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			player->PlayerMoveAttempt(Vector2(-plDirX, -plDirY), map);
			return;
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			player->TurnPlayer(DEG_TO_RAD(-90));
			return;
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			player->TurnPlayer(DEG_TO_RAD(90));
			return;
		}
	}

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		m_gameIsRunning = false;
		return;
	}

}

void Game::CommandInput(string command, Player*& player, Camera*& camera, Map*& map)
{
	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	// Convert string to lowercase
	command = StringToLower(command);

	if (command == "move forward")
	{
		player->PlayerMoveAttempt(Vector2(plDirX, plDirY), map);
		return;
	}
	else if (command == "move backward")
	{
		player->PlayerMoveAttempt(Vector2(-plDirX, -plDirY), map);
		return;
	}
	else if (command == "turn left")
	{
		player->TurnPlayer(DEG_TO_RAD(-90));
		return;
	}
	else if (command == "turn right")
	{
		player->TurnPlayer(DEG_TO_RAD(90));
		return;
	}
	else if (command == "spell")
	{
		std::cout << "\033[2KPlease enter a spell to check whether you know it or not: ";
		std::getline(std::cin, command);
		std::cin.clear();

		command = StringToLower(command);

		std::cout << std::format
		(
			"\033[2KYou{} know {}!",
			(player->HasSpell(command)) ? "" : " don't",
			StringCapitalise(command)
		);
		return;
	}
	else if (command == "escape")
	{
		m_gameIsRunning = false;
	}
	else
	{

	}
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
			textureList[0]->SetTextureColor(x, y, Color( 255 * (x != y && x != textureSize.x - y) ).RGBToRGBA() ); // flat red texture w/ black cross
			textureList[1]->SetTextureColor(x, y, Color( xyColor + 245 * xyColor + 65536 * xyColor ).RGBToRGBA()); // sloped greyscale
			textureList[2]->SetTextureColor(x, y, Color( 256 * xyColor + 65536 * xyColor).RGBToRGBA()); // sloped yellow gradient
			textureList[3]->SetTextureColor(x, y, Color( xorColor + 256 * xorColor + 65536 * xorColor ).RGBToRGBA()); // xor greyscale
			textureList[4]->SetTextureColor(x, y, Color( 256 * xorColor).RGBToRGBA()); // xor green
			textureList[5]->SetTextureColor(x, y, Color( 255 * (x % 16 && y % 16) ).RGBToRGBA()); // red bricks
			textureList[6]->SetTextureColor(x, y, Color( 255 * yColor).RGBToRGBA()); // red gradient
			textureList[7]->SetTextureColor(x, y, Color( 8421504 ).RGBToRGBA()); // flat gray texture
		}
	}
}

void Game::MoveRoom(Vector2i direction)
{
	Vector2i newRoom = m_currentRoom + direction;
	
	/*
	If new room:
	1) Is not below 0 on x coordinate
	2) Is not above total x coords
	3) Is not below 0 on t coordinate
	4) Is not above total y coords
	*/
	if (newRoom.x >= 0 || newRoom.x < m_roomsSize.x || newRoom.y >= 0 || newRoom.y < m_roomsSize.y)
	{
		ChangeRoom(GetRoom(newRoom));
	}
}

void Game::ChangeRoom(Room* room)
{
	m_currentMap = room->GetMap();

	Vector2 direction = room->GetStartingDirection();

	Vector2 position = room->GetStartingPosition();

	m_player->position.x = position.x;
	m_player->position.y = position.y;


	m_player->direction.x = direction.x;
	m_player->direction.y = direction.y;

	Camera* camera = m_player->GetCamera();

	float cameraFov = (camera->size.x + camera->size.y);

	camera->size.x = direction.y * abs(cameraFov);
	camera->size.y = direction.x * abs(cameraFov);
}

Room* Game::GetRoom(Vector2i position)
{
	return m_rooms[position.y][position.x];
}

void Game::SetRoom(Vector2i position, Room* room)
{
	/*
	If position:
	1) Is not below 0 on x coordinate
	2) Is not above total x coords
	3) Is not below 0 on t coordinate
	4) Is not	 above total y coords
	*/
	if (position.x >= 0 || position.x < m_roomsSize.x || position.y >= 0 || position.y < m_roomsSize.y)
	{
		m_rooms[position.y][position.x] = room;
		m_currentRoom = position;
	}
}
