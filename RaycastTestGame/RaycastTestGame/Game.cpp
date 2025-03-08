#include "Game.h"

#include <cmath>
#include <ctime>
#include <iostream>
#include <format>
#include <string>
#include <vector>

// ACTUAL GOOD NEEDED HEADERS

#include "ConsoleUtils.h"
#include "Map.h"
#include "Player.h"
#include "Raycast.h"
#include "Sprite.h"
#include "Texture.h"
#include "Viewport.h"

// ASSESMENT HEADERS

#include "Item.h"
#include "BoxOfDonuts.h"
#include "Cat.h"
#include "Lamp.h"

#include "Room.h"

/* TO DO
NEEDED:


OPTIONAL:
	LIGHTING SYSTEM
	TEXTURE MANAGER
	FULLY TYPELESS TWEENS
	ADD MAP EDITOR / CREATOR
	ADD SETTINGS AS BITWISE VALUE
*/

using std::vector;

Game::Game()
	: m_oldTime{ 0.f }, m_time{ 0.f }, m_deltaTime{ 0.f }, 
	m_gameIsRunning{ true },  m_player{ nullptr }, m_currentMap{ nullptr },
	m_currentRoom{ nullptr }, m_rooms{ nullptr }, m_useCommandInput{ true }
{

}

int Game::Run()
{
	int exitCode;
	
	exitCode = BeginPlay();

	// Checks for any start errors
	if (exitCode != 0)
	{
		return exitCode;
	}

	while (m_gameIsRunning)
	{
		// Gets the delta time by comparing the current time to the last time and convert into seconds
		m_deltaTime = (clock() - m_oldTime) / 1000.f;
		m_oldTime = clock();

		exitCode = Tick(m_deltaTime);

		// Checks for any tick errors
		if (exitCode != 0)
		{
			return exitCode;
		}

	}

	// Checks for any ending errors
	exitCode = EndPlay();

	return exitCode;
	
}

int Game::BeginPlay()
{

	// Set rand seed to time
	srand(time(nullptr));

	/*
		Creating Textures
	*/

	// Set the resolution for proceedual textures
	Vector2i defaultTextureSize = Vector2i(64, 64);

	// Create default proceedual textures
	//CreateDefaultTextures(m_textureList, defaultTextureSize);

	//for (int i = 0; i < 8; i++)
	//{
	//	m_textureList[i]->SetTexture("images\\coffeecup.png");
	//}

	// Emplace textures
	m_textureList.emplace_back(new Texture()); // Error Texture at 0

	m_textureList.emplace_back(new Texture("images\\adachitrue.jpeg"));
	m_textureList.emplace_back(new Texture("images\\adachifalse.jpeg"));
	m_textureList.emplace_back(new Texture("images\\adachimabye.jpeg"));

	m_textureList.emplace_back(new Texture("images\\tart_wall.png"));
	m_textureList.emplace_back(new Texture("images\\tart_floor.png"));
	m_textureList.emplace_back(new Texture("images\\tart_roof.png"));

	m_textureList.emplace_back(new Texture("images\\bonafiedmonafied.png"));

	m_textureList.emplace_back(new Texture("images\\trollface.png"));

	m_textureList.emplace_back(new Texture("images\\wolftex\\eagle.png"));
	m_textureList.emplace_back(new Texture("images\\wolftex\\redbrick.png"));
	m_textureList.emplace_back(new Texture("images\\wolftex\\purplestone.png"));
	m_textureList.emplace_back(new Texture("images\\wolftex\\greystone.png"));
	m_textureList.emplace_back(new Texture("images\\wolftex\\bluestone.png"));
	m_textureList.emplace_back(new Texture("images\\wolftex\\mossy.png"));
	m_textureList.emplace_back(new Texture("images\\wolftex\\wood.png"));
	m_textureList.emplace_back(new Texture("images\\wolftex\\colorstone.png"));
	m_textureList.emplace_back(new Texture("images\\wolftex\\barrel.png"));
	m_textureList.emplace_back(new Texture("images\\wolftex\\pillar.png"));
	m_textureList.emplace_back(new Texture("images\\wolftex\\greenlight.png"));

	m_textureList.emplace_back(new Texture("images\\coffeecup.png"));
	m_textureList.emplace_back(new Texture("images\\lobotomy.jpeg"));
	m_textureList.emplace_back(new Texture("images\\smalladachi.png"));

	
	////THIS WAS FOR TESTING TEXTURES LOADING PROPERLY
	////KEEPING COMMENTED JUST IN CASE IT IS NEEDED IN THE FUTURE

	//Texture testTexture = Texture("images\\bonafiedmonafied.png");

	//Viewport* testViewport = new Viewport();

	//testViewport->SetColorABuffer(testTexture.GetSize(), testTexture.GetTexture());

	//DrawColorViewport(testViewport);

	//delete testViewport;

	//return 0;

	// Create Rooms
	{
		// Create 2D Array of Room pointers
		m_roomsSize = Vector2i(2, 2);

		m_rooms = new Room * *[m_roomsSize.y];

		for (int y = 0; y < m_roomsSize.y; y++)
		{
			m_rooms[y] = new Room * [m_roomsSize.x];
			for (int x = 0; x < m_roomsSize.x; x++)
			{
				m_rooms[y][x] = nullptr;
			}
		}

		// Create Room 1

		Room* room1 = new Room();

		// Create Map
		// Wall data of the map
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

		// Create truncated 2D of ints as this is how map data reads information
		uint16_t* tempMapData = new uint16_t[24 * 24];
		for (int i = 0; i < 24 * 24; i++)
		{
			tempMapData[i] = tempMapWall[i];
		}

		// Create new map
		room1->SetMap(new Map(24, 24));

		// Set the wall contents of the map
		room1->GetMap()->SetContentDataType(tempMapData, MapDataType::WALL, Vector2i(24, 24));

		// Delete 2D array as data is assigned to map
		delete[] tempMapData;

		// Set roof and floor data to checkerboard pattern
		for (int x = 0; x < 24; x++)
		{
			for (int y = 0; y < 24; y++)
			{
				room1->GetMap()->SetContentsFromLocation(x, y, ((x + y) % 2 == 0) ? 1 : 2, MapDataType::FLOOR);
				room1->GetMap()->SetContentsFromLocation(x, y, ((x + y) % 2 == 0) ? 1 : 2, MapDataType::ROOF);
			}
		}

		// Set player starting position and direction
		room1->SetStartingPosition(Vector2(22.5f, 1.5f));
		room1->SetStartingDirection(Vector2(-1.f, 0.f));

		// Set description of the room
		room1->SetDescription(string("Basic Room :D"));

		// Set position of the room on the map
		room1->SetRoomPosition(Vector2i(0, 0));

		// Add Sprites to map

		// Create sprite* so the sprite properties can be edited
		Sprite* sprite;

		// Set the sprite pointer to a new sprite in the position in the map and texture
		sprite = new Sprite(Vector2(20.5f, 11.5f), m_textureList[19]);

		// Set Y offset and scale of sprite
		sprite->SetYOffset(-40.f);
		sprite->SetScale(Vector2(2, 2));

		// Add sprites to map
		room1->GetMap()->AddSprite(sprite);
		room1->GetMap()->AddSprite(new Sprite(Vector2(18.5f, 4.5f), m_textureList[19]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(10.5f, 4.5f), m_textureList[19]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(10.5f, 12.5f), m_textureList[19]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(3.5f, 6.5f), m_textureList[19]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(3.5f, 20.5f), m_textureList[19]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(3.5f, 14.5f), m_textureList[19]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(14.5f, 20.5f), m_textureList[19]));

		room1->GetMap()->AddSprite(new Sprite(Vector2(18.5f, 10.5f), m_textureList[18]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(18.5f, 11.5f), m_textureList[18]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(18.5f, 12.5f), m_textureList[18]));

		room1->GetMap()->AddSprite(new Sprite(Vector2(21.5f, 1.5f), m_textureList[17]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(15.5f, 1.5f), m_textureList[17]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(16.0f, 1.8f), m_textureList[17]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(16.2f, 1.2f), m_textureList[17]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(3.5f, 2.5f), m_textureList[17]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(9.5f, 15.5f), m_textureList[17]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(10.0f, 15.1f), m_textureList[17]));
		room1->GetMap()->AddSprite(new Sprite(Vector2(10.5f, 15.8f), m_textureList[17]));

		// Set textures for room 1
		room1->GetMap()->EmplaceLayerTexture(9, MapDataType::WALL);
		room1->GetMap()->EmplaceLayerTexture(10, MapDataType::WALL);
		room1->GetMap()->EmplaceLayerTexture(11, MapDataType::WALL);
		room1->GetMap()->EmplaceLayerTexture(12, MapDataType::WALL);
		room1->GetMap()->EmplaceLayerTexture(13, MapDataType::WALL);
		room1->GetMap()->EmplaceLayerTexture(14, MapDataType::WALL);
		room1->GetMap()->EmplaceLayerTexture(15, MapDataType::WALL);
		room1->GetMap()->EmplaceLayerTexture(16, MapDataType::WALL);

		room1->GetMap()->EmplaceLayerTexture(12, MapDataType::FLOOR);
		room1->GetMap()->EmplaceLayerTexture(14, MapDataType::FLOOR);

		room1->GetMap()->EmplaceLayerTexture(10, MapDataType::ROOF);
		room1->GetMap()->EmplaceLayerTexture(11, MapDataType::ROOF);

		// Add item to the room
		room1->AddRandomItem();

		// Set the room in the Room 2D array
		SetRoom(room1);

		// Create Room2

		/*
		
			ALL THE SAME STUFF BUT REPEATED FOR EACH ROOM
		
		*/

		Room* room2 = new Room();

		uint16_t temp2MapWall[5 * 5] =
		{
			1,1,1,1,1,
			1,0,0,0,1,
			1,0,0,0,1,
			1,0,0,0,1,
			1,1,1,1,1
		};

		tempMapData = new uint16_t[5 * 5];
		for (int i = 0; i < 5 * 5; i++)
		{
			tempMapData[i] = temp2MapWall[i];
		}

		room2->SetMap(new Map(5, 5));

		room2->GetMap()->SetContentDataType(tempMapData, MapDataType::WALL, Vector2i(5, 5));

		// Set roof and floor data to 1
		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 5; y++)
			{
				room2->GetMap()->SetContentsFromLocation(x, y, 1, MapDataType::FLOOR);
				room2->GetMap()->SetContentsFromLocation(x, y, 1, MapDataType::ROOF);
			}
		}

		delete[] tempMapData;

		room2->GetMap()->AddSprite(new Sprite(Vector2(2.5f, 2.5f), m_textureList[1]));

		room2->SetStartingPosition(Vector2(2.5f, 3.5f));
		room2->SetStartingDirection(Vector2(0.f, -1.f));

		room2->SetDescription(string("What is this hell...?"));

		room2->SetRoomPosition(Vector2i(1, 0));

		// Set texture for each number (1 in this context)
		room2->GetMap()->EmplaceLayerTexture(1, MapDataType::WALL);
		room2->GetMap()->EmplaceLayerTexture(2, MapDataType::FLOOR);
		room2->GetMap()->EmplaceLayerTexture(3, MapDataType::ROOF);

		SetRoom(room2);

		// Create Room 3

		Room* room3 = new Room();

		uint16_t temp3MapWall[3 * 3] =
		{
			1,1,1,
			1,0,1,
			1,1,1
		};

		tempMapData = new uint16_t[3*3];
		for (int i = 0; i < 3*3; i++)
		{
			tempMapData[i] = temp3MapWall[i];
		}

		room3->SetMap(new Map(3, 3));

		room3->GetMap()->SetContentDataType(tempMapData, MapDataType::WALL, Vector2i(3, 3));

		room3->AddRandomItem();

		delete[] tempMapData;

		// Set roof and floor data to 1
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				room3->GetMap()->SetContentsFromLocation(x, y, 1, MapDataType::FLOOR);
				room3->GetMap()->SetContentsFromLocation(x, y, 1, MapDataType::ROOF);
			}
		}

		room3->GetMap()->EmplaceLayerTexture(8, MapDataType::WALL);
		room3->GetMap()->EmplaceLayerTexture(8, MapDataType::FLOOR);
		room3->GetMap()->EmplaceLayerTexture(8, MapDataType::ROOF);

		room3->SetStartingPosition(Vector2(1.5f, 1.5f));
		room3->SetStartingDirection(Vector2(1.f, 0.f));

		room3->SetDescription(string("Trollge"));

		room3->SetRoomPosition(Vector2i(0, 1));

		SetRoom(room3);

		// Create Room 4
		Room* room4 = new Room();

		uint16_t temp4MapWall[7 * 7] =
		{
			1,1,1,1,1,1,1,
			1,0,0,0,0,0,1,
			1,0,0,0,0,0,1,
			1,0,0,0,0,0,1,
			1,0,0,0,0,0,1,
			1,0,0,0,0,0,1,
			1,1,1,1,1,1,1
		};

		tempMapData = new uint16_t[7 * 7];
		for (int i = 0; i < 7 * 7; i++)
		{
			tempMapData[i] = temp4MapWall[i];
		}

		room4->SetMap(new Map(7, 7));

		room4->GetMap()->SetContentDataType(tempMapData, MapDataType::WALL, Vector2i(7, 7));

		room4->AddItem(new Cat());

		delete[] tempMapData;

		room4->GetMap()->EmplaceLayerTexture(4, MapDataType::WALL);
		room4->GetMap()->EmplaceLayerTexture(5, MapDataType::FLOOR);
		room4->GetMap()->EmplaceLayerTexture(6, MapDataType::ROOF);

		for (int x = 0; x < 7; x++)
		{
			for (int y = 0; y < 7; y++)
			{
				room4->GetMap()->SetContentsFromLocation(x, y, 1, MapDataType::FLOOR);
				room4->GetMap()->SetContentsFromLocation(x, y, 1, MapDataType::ROOF);
			}
		}

		room4->GetMap()->AddSprite(new Sprite(Vector2(3.5f, 3.5f), m_textureList[7]));

		room4->SetStartingPosition(Vector2(5.5f, 5.5f));
		room4->SetStartingDirection(Vector2(0.f, -1.f));

		room4->SetDescription(string("bonafied monafied B)"));

		room4->SetRoomPosition(Vector2i(1, 1));

		SetRoom(room4);
	}

	// Create Player Camera & Viewport

	m_player = new Player
	(
		Vector2(22.5f, 1.5f), Vector2(-1.f, 0.f),  // Player Data
		Vector2(0.f, -1.4f), // Camera Data
		Vector2i(10, 3), Vector2i(128, 32) // Viewport Data
	);


	// Console Settings

	// Set the buffer size of the console (how many characters it can display at once
	SetConsoleBufferResolution(1024, 1024);

	// Hides the cursor from the console
	SetCursorVis(false);

	// Enables ANSI Escape Keys which allows for RGB color backgrounds
	ToggleANSI(true);

	// Get viewport & camera
	Camera* camera = m_player->GetCamera();
	Viewport* viewport = camera->GetViewport();

	// Move the console cursor to the end of the viewport so that it 
	// doesn't get overwritten when the viewport is drawn
	SetConsoleCursorPos(0, (viewport->size.y + viewport->position.y + 3));

	// Set Beginner Room

	ChangeRoom(GetRoomFromPos(Vector2i(0, 0)));

	// First Frame Draw

	Raycaster(viewport, m_player, camera, m_currentMap, m_textureList);
	DrawColorViewport(viewport);

	return EXIT_SUCCESS;
}

int Game::EndPlay()
{
	// Delete all textures
	for (int i = 0; i < m_textureList.size(); i++)
	{
		if (m_textureList[i] != nullptr)
		{
			delete m_textureList[i];
		}
	}

	// Delete all Rooms
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
	// Define camera and
	Camera* mainCam = m_player->GetCamera();
	Viewport* mainViewport = mainCam->GetViewport();

	int& width = mainViewport->size.x;
	int& height = mainViewport->size.y;

	unsigned int fps = (1.f / deltaTime);

	// Run the players tweens to interpolate movement
	m_player->RunTweens(deltaTime);

	// If using command input
	if (m_useCommandInput)
	{
		// Check if player is not moving
		if (m_player->IsMoving() == false)
		{
			// Make cursor visible
			SetCursorVis(true);
			
			// create new stream command
			string command;

			// Set console cursor pos to 2 rows below viewport bottom
			SetConsoleCursorPos(0, (height + mainViewport->position.y + 1));

			std::cout << "\033[2K"; // Erase current line

			std::cout << "Please enter input: ";

			// Get the current line of the player input and clear input stream
			std::getline(std::cin, command);
			std::cin.clear();

			// Reset old time not to account for time waiting typing to delta
			m_oldTime = clock();

			// Move console 4 rows below console
			SetConsoleCursorPos(0, (height + mainViewport->position.y + 3));

			// Call comand input function
			CommandInput(command, m_player, mainCam, m_currentMap);

			// Make cursor invisible
			SetCursorVis(false);
		}
	}
	else
	{
		//OldKeyboardInput(m_player, mainCam, m_currentMap, m_deltaTime);
		KeyboardInput(m_player, mainCam, m_currentMap);
	}

	// Print out data to console
	SetConsoleCursorPos(0, (height + mainViewport->position.y)); // Move position of the cursor
	std::cout << "\033[2K"; // Erase current line
	std::cout << "FPS: " << fps; // Output fps

	SetConsoleCursorPos(0, (height + mainViewport->position.y + 1));
	std::cout << "\033[2K"; // Erase current line
	std::cout << std::format("Player Position: [{}, {}]", int(m_player->position.x), int(m_player->position.y));

	SetConsoleCursorPos(0, (height + mainViewport->position.y + 2));
	std::cout << "\033[2K"; // Erase current line
	std::cout << std::format("Player Direction: [{}, {}]", m_player->direction.x, m_player->direction.y);

	//mainViewport->ClearViewport(true);

	// Run Raycaster
	Raycaster(mainViewport, m_player, mainCam, m_currentMap, m_textureList);


	//DrawASCIIViewport(mainViewport);
	DrawColorViewport(mainViewport);

	return 0;
}

void Game::Raycaster(Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*> textures)
{
	SetConsoleBufferResolution(1024, 1024);
	// Raycasting Loop

	// Create Zbuffer for sprite so it can find the walls distance to the camera
	float* zBuffer = new float[viewport->size.x];

	// Run floor raycast for every row
	for (int y = 0; y < viewport->size.y; y++)
	{
		FloorRaycast(y, viewport, player, camera, map, textures);
	}
	
	// Run wall raycast for ever column
	for (int x = 0; x < viewport->size.x; x++)
	{
		WallRaycast(x, viewport, player, camera, map, textures, zBuffer);
	}

	// Cast sprite
	SpriteCasting(viewport, player, camera, textures, map, zBuffer);

	// Delete 2 Buffer
	delete[] zBuffer;
}

void Game::OldKeyboardInput(Player*& player, Camera*& camera, Map*& map, float deltaTime)
{
	// OLD KEYBOARD MOVEMENT THAT WASN'T LINEAR GRID BASED MOVEMENT
	// DOESNT USE TWEENS

	// Get rotation & movement speed
	float moveSpeed = deltaTime * 5.0f; // Cells per second
	float rotSpeed = deltaTime * 3.0f; // radians / second

	// Reference vars for cleaner code
	float& plPosX = player->position.x;
	float& plPosY = player->position.y;

	float& plDirX = player->direction.x;
	float& plDirY = player->direction.y;

	// Get the wall data of the map to detect collision
	uint16_t* wallData = map->GetDataTypeBuffer(MapDataType::WALL);
	Vector2i mapSize = map->GetMapSize();

	// Keyboard Inputs
	// Move Forward if not crash into wall
	if (GetAsyncKeyState(VK_UP))
	{
		// If x + direction or y + direction * movement speed  doesnt interact with wall
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
		// If x - direction or y - direction * movement speed  doesnt interact with wall
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
		// Rotate Camera & Player
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
		// Rotate Camera & Player
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
		// Stop game running
		m_gameIsRunning = false;
	}

	// delete wall data
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
		// Exit game
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

	// Map Movement

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

	// Room Movement
	else if (command == "move room north")
	{
		// Get room adjacent to direction
		Room* room = GetRoomFromPos(m_currentRoom->GetPos() + Vector2i(0, 1));

		// If room exists
		if (room != nullptr)
		{
			ChangeRoom(room);
		}
		else
		{
			std::cout << "\033[2KInvalid Room!";
		}
	}
	else if (command == "move room south")
	{
		// Get room adjacent to direction
		Room* room = GetRoomFromPos(m_currentRoom->GetPos() + Vector2i(0, -1));

		// If room exists
		if (room != nullptr)
		{
			ChangeRoom(room);
		}
		else
		{
			std::cout << "\033[2KInvalid Room!";
		}
	}
	else if (command == "move room east")
	{
		// Get room adjacent to direction
		Room* room = GetRoomFromPos(m_currentRoom->GetPos() + Vector2i(1, 0));

		// If room exists
		if (room != nullptr)
		{
			ChangeRoom(room);
		}
		else
		{
			std::cout << "\033[2KInvalid Room!";
		}
	}
	else if (command == "move room west")
	{
		// Get room adjacent to direction
		Room* room = GetRoomFromPos(m_currentRoom->GetPos() + Vector2i(-1, 0));

		// If room exists
		if (room != nullptr)
		{
			ChangeRoom(room);
		}
		else
		{
			std::cout << "\033[2KInvalid Room!";
		}
	}

	// Spells 
	else if (command == "spell")
	{
		// Get input
		std::cout << "\033[2KPlease enter a spell to check whether you know it or not: ";
		std::getline(std::cin, command);
		std::cin.clear();

		// convert command to lowercase
		command = StringToLower(command);

		std::cout << std::format
		(
			"\033[2KYou{} know {}!",
			(player->FindSpell(command)) ? "" : " don't", // Find Spell
			StringCapitalise(command) // Capitalise command
		);
		return;
	}

	// Interaction and Description

	else if (command == "room desc")
	{
		std::cout << "\033[2K";
		m_currentRoom->Description();
		return;
	}
	else if (command == "item interact")
	{
		std::cout << "\033[2K";

		// Get item in the room
		Item* item = m_currentRoom->GetItem();

		// If item exists
		if (item != nullptr)
		{
			item->Use();
		}
		else
		{
			std::cout << "There is no item in this room!";
		}
		return;
		
	}
	else if (command == "item desc")
	{
		std::cout << "\033[2K";

		// Get item in the room
		Item* item = m_currentRoom->GetItem();

		// If item exists
		if (item != nullptr)
		{
			item->Description();
		}
		else
		{
			std::cout << "There is no item in this room!";
		}
		return;
	}

	// Exiting and Window Handling

	else if (command == "clear screen")
	{
		// Clears entire console
		system("CLS");
	}
	else if (command == "escape")
	{
		// Exit game
		m_gameIsRunning = false;
		std::cout << "\033[2KExiting game...";
	}
	else
	{
		std::cout << std::format("\033[2KCommand '{}' is invalid! Please try again!", command);
	}
}

void Game::CreateDefaultTextures(vector<Texture*>& textureList, Vector2i textureSize)
{
	// Create new textures and add them to texture vector
	for (int i = 0; i < 8; i++)
	{
		Texture* texture = new Texture();
		texture->CreateNewTexture(textureSize);
		textureList.emplace_back(texture);
	}

	// Create proceedual textures by passing int as color and converting RGB TO RGBA
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

void Game::ChangeRoom(Room* room)
{
	// If room exists
	if (room != nullptr)
	{
		// Set current room
		m_currentRoom = room;

		// Change map
		m_currentMap = room->GetMap();

		// Get player starting position and direction
		Vector2 direction = room->GetStartingDirection();
		Vector2 position = room->GetStartingPosition();

		// Set player position
		m_player->position.x = position.x;
		m_player->position.y = position.y;

		// Set player direction
		m_player->direction.x = direction.x;
		m_player->direction.y = direction.y;

		// Get Camera
		Camera* camera = m_player->GetCamera();

		// Get camera fov
		float cameraFov = abs(camera->size.x) + abs(camera->size.y);

		// Set camera direction
		camera->size.x = -direction.y * abs(cameraFov);
		camera->size.y = direction.x * abs(cameraFov);

		// Get room item
		Item* item = room->GetItem();

		// If item exists
		if (item != nullptr)
		{
			item->Description();
		}
		else
		{
			std::cout << "\033[2KThe room is empty!";
		}

		std::cout << "\n";

		room->Description();
	}
}

Room* Game::GetRoomFromPos(Vector2i position)
{
	/*
	If position:
	1) Is not below 0 on x coordinate
	2) Is not above total x coords
	3) Is not below 0 on t coordinate
	4) Is not	 above total y coords
	*/
	if (position.x >= 0 && position.x < m_roomsSize.x && position.y >= 0 && position.y < m_roomsSize.y)
	{
		return m_rooms[position.y][position.x];
	}
	
	return nullptr;
}

void Game::SetRoom(Room* room)
{
	Vector2i position = room->GetPos();
	/*
	If position:
	1) Is not below 0 on x coordinate
	2) Is not above total x coords
	3) Is not below 0 on t coordinate
	4) Is not above total y coords
	*/
	if (position.x >= 0 && position.x < m_roomsSize.x && position.y >= 0 && position.y < m_roomsSize.y)
	{
		m_rooms[position.y][position.x] = room;
	}
}
