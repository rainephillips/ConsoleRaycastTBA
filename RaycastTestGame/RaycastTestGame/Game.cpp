#include "Game.h"

#include <iostream>
#include <format>

// ACTUAL GOOD NEEDED HEADERS

#include "ConsoleUtils.h"
#include "Map.h"
#include "Player.h"
#include "Camera.h"
#include "Raycast.h"
#include "Sprite.h"
#include "Texture.h"
#include "Viewport.h"

// ASSESMENT HEADERS

#include "Cat.h"

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
	: m_oldTime{ 0.f }, m_deltaTime{ 0.f }, 
	m_gameIsRunning{ true },  m_player{ nullptr }, m_currentMap{ nullptr },
	m_currentRoom{ nullptr }, m_rooms{ nullptr }
{
	m_useCommandInput = true;
	m_playerFOV = 1.3f;
	m_mainViewportOffset = Vector2i{ 10, 3 };
	m_viewportResolution = { 128, 64 };
	m_screenBufferResolution = Vector2i{ 512, 512 };
	m_beginnerRoomPos = Vector2i{ 0, 0 };
	m_roomsSize = { 2, 2 };

	m_mapData = vector<MapData>();
	m_mapData.emplace_back
	(
		MapData
		{
			Vector2i{ 24, 24 },
			vector<uint64_t>{4295032840, 8590065672, 4295032840, 8590065672, 4295032840, 8590065672, 4295032840, 8590065672, 4295032840, 8590065672, 4295032840, 8590065668, 4295032836, 8590065670, 4295032836, 8590065668, 4295032838, 8590065668, 4295032838, 8590065668, 4295032836, 8590065668, 4295032838, 8590065668, 8590065672, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065672, 4295032836, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032836, 4295032840, 8590065664, 4295032835, 8590065667, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065672, 4295032840, 8590065668, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065670, 8590065672, 4295032832, 8590065664, 4295032835, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032838, 4295032840, 8590065664, 4295032835, 8590065667, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065672, 4295032840, 8590065668, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065668, 8590065672, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065672, 4295032836, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032838, 8590065670, 4295032838, 8590065664, 4295032838, 8590065668, 4295032838, 4295032840, 8590065672, 4295032840, 8590065672, 4295032832, 8590065672, 4295032840, 8590065672, 4295032840, 8590065672, 4295032840, 8590065668, 4295032836, 8590065668, 4295032836, 8590065668, 4295032836, 8590065670, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065670, 8590065671, 4295032839, 8590065671, 4295032839, 8590065664, 4295032839, 8590065671, 4295032839, 8590065671, 4295032832, 8590065672, 4295032832, 8590065672, 4295032832, 8590065672, 4295032832, 8590065672, 4295032836, 8590065664, 4295032836, 8590065664, 4295032838, 8590065664, 4295032838, 4295032839, 8590065671, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032839, 8590065672, 4295032832, 8590065672, 4295032832, 8590065672, 4295032832, 8590065672, 4295032840, 8590065670, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065670, 8590065671, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065672, 4295032838, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032836, 4295032839, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032840, 8590065670, 4295032832, 8590065670, 4295032832, 8590065670, 4295032832, 8590065670, 8590065671, 4295032839, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065671, 4295032840, 8590065664, 4295032840, 8590065664, 4295032840, 8590065664, 4295032840, 8590065672, 4295032838, 8590065668, 4295032838, 8590065664, 4295032838, 8590065670, 4295032838, 4295032839, 8590065671, 4295032839, 8590065671, 4295032832, 8590065671, 4295032839, 8590065671, 4295032839, 8590065672, 4295032840, 8590065668, 4295032832, 8590065670, 4295032840, 8590065668, 4295032840, 8590065667, 4295032835, 8590065667, 4295032832, 8590065667, 4295032835, 8590065667, 8590065666, 4295032834, 8590065666, 4295032834, 8590065664, 4295032834, 8590065666, 4295032834, 8590065666, 4295032836, 8590065670, 4295032836, 8590065664, 4295032832, 8590065670, 4295032832, 8590065670, 4295032835, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032835, 4295032834, 8590065666, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065666, 4295032834, 8590065668, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032836, 8590065667, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065667, 8590065666, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065666, 4295032836, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065668, 4295032835, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032835, 4295032833, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032833, 8590065668, 4295032836, 8590065668, 4295032836, 8590065668, 4295032838, 8590065664, 4295032838, 8590065667, 4295032835, 8590065664, 4295032832, 8590065664, 4295032835, 8590065667, 8590065666, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065666, 4295032834, 8590065666, 4295032833, 8590065666, 4295032834, 8590065666, 4295032838, 8590065670, 4295032832, 8590065664, 4295032837, 8590065664, 4295032837, 8590065664, 4295032837, 4295032834, 8590065666, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065666, 4295032834, 8590065666, 4295032832, 8590065664, 4295032832, 8590065666, 4295032834, 8590065664, 4295032837, 8590065664, 4295032837, 8590065664, 4295032832, 8590065664, 4295032837, 8590065669, 8590065666, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065666, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065666, 4295032837, 8590065664, 4295032837, 8590065664, 4295032837, 8590065664, 4295032837, 8590065664, 4295032837, 4295032833, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065669, 8590065666, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065666, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065666, 4295032837, 8590065664, 4295032837, 8590065664, 4295032837, 8590065664, 4295032837, 8590065664, 4295032837, 4295032834, 8590065666, 4295032832, 8590065664, 4295032832, 8590065664, 4295032832, 8590065666, 4295032834, 8590065666, 4295032832, 8590065664, 4295032832, 8590065666, 4295032834, 8590065664, 4295032837, 8590065664, 4295032837, 8590065664, 4295032832, 8590065664, 4295032837, 8590065669, 8590065666, 4295032834, 8590065666, 4295032834, 8590065665, 4295032834, 8590065666, 4295032834, 8590065666, 4295032834, 8590065666, 4295032833, 8590065666, 4295032834, 8590065666, 4295032837, 8590065669, 4295032837, 8590065669, 4295032837, 8590065669, 4295032837, 8590065669, 4295032837, },
			Vector2{ 22.5f, 1.5f },
			Vector2{ 0.f, -1.f },
			Vector2i{ 0, 0 },
			string(""),
			vector<SpriteData>{ SpriteData{ Vector2{ 20.5f, 11.5f }, 19, Vector2{ 2.f, 2.f}, -40.f }, SpriteData{ Vector2{ 18.5f, 4.5f }, 19 }, SpriteData{ Vector2{ 10.5f, 4.5f }, 19 }, SpriteData{ Vector2{ 10.5f, 12.5f }, 19 }, SpriteData{ Vector2{ 3.5f, 6.5f }, 19 }, SpriteData{ Vector2{ 3.5f, 20.5f }, 19 }, SpriteData{ Vector2{ 3.5f, 14.5f }, 19 }, SpriteData{ Vector2{ 14.5f, 20.5f }, 19 }, SpriteData{ Vector2{ 18.5f, 10.5f }, 18 }, SpriteData{ Vector2{ 18.5f, 11.5f }, 18 }, SpriteData{ Vector2{ 18.5f, 12.5f }, 18 }, SpriteData{ Vector2{ 21.5f, 1.5f }, 17 }, SpriteData{ Vector2{ 15.5f, 1.5f }, 17 }, SpriteData{ Vector2{ 16.0f, 1.8f }, 17 }, SpriteData{ Vector2{ 16.2f, 1.2f }, 17 }, SpriteData{ Vector2{ 3.5f, 2.5f }, 17 }, SpriteData{ Vector2{ 9.5f, 15.5f }, 17 }, SpriteData{ Vector2{ 10.0f, 15.1f }, 17 }, SpriteData{ Vector2{ 10.5f, 15.8f }, 17 }},
			vector<size_t>{9, 10, 11, 12, 13, 14, 15, 16},
			vector<size_t>{12, 14},
			vector<size_t>{10, 11},
			nullptr
		}
	);

	m_mapData.emplace_back
	(
		MapData
		{
			Vector2i{5, 5},
			vector<uint64_t>{4295032833, 4295032833, 4295032833, 4295032833, 4295032833, 4295032833, 4295032832, 4295032832, 4295032832, 4295032833, 4295032833, 4295032832, 4295032832, 4295032832, 4295032833, 4295032833, 4295032832, 4295032832, 4295032832, 4295032833, 4295032833, 4295032833, 4295032833, 4295032833, 4295032833},
			Vector2{ 2.5f, 3.5f },
			Vector2{ 0.f, -1.f },
			Vector2i{ 1, 0 },
			string("What is this hell...?"),
			vector<SpriteData>{SpriteData{Vector2{ 2.5f, 2.5f }, 1}},
			vector<size_t>{1},
			vector<size_t>{2},
			vector<size_t>{3},
			nullptr
		}
	);

	m_mapData.emplace_back
	(
		MapData
		{
			Vector2i{3, 3},
			vector<uint64_t>{4295032833, 4295032833, 4295032833, 4295032833, 4295032832, 4295032833, 4295032833, 4295032833, 4295032833},
			Vector2{1.5f, 1.5f},
			Vector2{ 1.f, 0.f },
			Vector2i{ 0, 1 },
			string("Trollge"),
			vector<SpriteData>{},
			vector<size_t>{8},
			vector<size_t>{8},
			vector<size_t>{8},
			nullptr
		}
	);

	m_mapData.emplace_back
	(
		MapData
		{
			Vector2i{7, 7},
			vector<uint64_t>{4295032833, 4295032833, 4295032833, 4295032833, 4295032833, 4295032833, 4295032833, 4295032833, 4295032832, 4295032832, 4295032832, 4295032832, 4295032832, 4295032833, 4295032833, 4295032832, 4295032832, 4295032832, 4295032832, 4295032832, 4295032833, 4295032833, 4295032832, 4295032832, 4295032832, 4295032832, 4295032832, 4295032833, 4295032833, 4295032832, 4295032832, 4295032832, 4295032832, 4295032832, 4295032833, 4295032833, 4295032832, 4295032832, 4295032832, 4295032832, 4295032832, 4295032833, 4295032833, 4295032833, 4295032833, 4295032833, 4295032833, 4295032833, 4295032833},
			Vector2{ 5.5f, 5.5f },
			Vector2{ 0.f, -1.f },
			Vector2i{1, 1},
			string("bonafied monafied B)"),
			vector<SpriteData>{SpriteData{Vector2{ 3.5f, 3.5f }, 7}},
			vector<size_t>{4},
			vector<size_t>{5},
			vector<size_t>{6},
			new Cat()
		}
	);

	m_texturePaths = vector<string>
	{
		"images\\adachitrue.jpeg",
		"images\\adachifalse.jpeg",
		"images\\adachimabye.jpeg",
		"images\\tart_wall.png",
		"images\\tart_floor.png",
		"images\\tart_roof.png",
		"images\\bonafiedmonafied.png",
		"images\\trollface.png",
		"images\\wolftex\\eagle.png",
		"images\\wolftex\\redbrick.png",
		"images\\wolftex\\purplestone.png",
		"images\\wolftex\\greystone.png",
		"images\\wolftex\\bluestone.png",
		"images\\wolftex\\mossy.png",
		"images\\wolftex\\wood.png",
		"images\\wolftex\\colorstone.png",
		"images\\wolftex\\barrel.png",
		"images\\wolftex\\pillar.png",
		"images\\wolftex\\greenlight.png",
		"images\\coffeecup.png",
		"images\\lobotomy.jpeg",
		"images\\smalladachi.png"
	};
}

int Game::Run()
{
	int exitCode = BeginPlay();

	// Checks for any start errors
	if (exitCode != 0)
	{
		return exitCode;
	}

	while (m_gameIsRunning)
	{
		// Gets the delta time by comparing the current time to the last time and convert into seconds
		m_deltaTime = (static_cast<float>(clock()) - m_oldTime) / 1000.f;
		m_oldTime = static_cast<float>(clock());

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


	// Emplace textures
	m_textureList.emplace_back(new Texture()); // Error Texture at 0

	for (string const& texturePath: m_texturePaths)
	{
		m_textureList.emplace_back(new Texture{ texturePath.c_str() });
	}

	// Create Rooms

	// Create 2D Array of Room pointers

	m_rooms = new Room * *[m_roomsSize.y];

	for (int y = 0; y < m_roomsSize.y; y++)
	{
		m_rooms[y] = new Room * [m_roomsSize.x];
		for (int x = 0; x < m_roomsSize.x; x++)
		{
			m_rooms[y][x] = nullptr;
		}
	}

	for (MapData map : m_mapData)
	{
		CreateRoom(map);
	}

	// Create Player Camera & Viewport

	m_player = new Player
	(
		Vector2{ 0.0f, 0.0f }, Vector2{ 0.0f, 0.f },  // Player Data
		Vector2{ 0.f, -m_playerFOV }, // Camera Data
		m_mainViewportOffset, m_viewportResolution // Viewport Data
	);


	// Console Settings

	// Set the buffer size of the console (how many characters it can display at once
	SetConsoleBufferResolution(m_screenBufferResolution.x, m_screenBufferResolution.y);

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

	ChangeRoom(GetRoomFromPos(m_beginnerRoomPos));

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

		m_textureList[i] = nullptr;
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

	int const& height = mainViewport->size.y;

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

			// Set console cursor pos to 2 rows below viewport bottom
			SetConsoleCursorPos(0, (height + mainViewport->position.y + 1));

			std::cout << "\033[2K"; // Erase current line

			std::cout << "Please enter input: ";

			// create new stream command
			string command;

			// Get the current line of the player input and clear input stream
			std::getline(std::cin, command);
			std::cin.clear();

			// Reset old time not to account for time waiting typing to delta
			m_oldTime = clock();

			// Move console 4 rows below console
			SetConsoleCursorPos(0, (height + mainViewport->position.y + 3));

			// Call comand input function
			CommandInput(command, m_player, m_currentMap);

			// Make cursor invisible
			SetCursorVis(false);
		}
	}
	else
	{
		KeyboardInput(m_player, m_currentMap);
	}

	// Print out data to console
	SetConsoleCursorPos(0, (height + mainViewport->position.y)); // Move position of the cursor
	std::cout << "\033[2K"; // Erase current line
	std::cout << "FPS: " << fps; // Output fps

	SetConsoleCursorPos(0, (height + mainViewport->position.y + 1));
	std::cout << "\033[2K"; // Erase current line
	std::cout << std::format("Player Position: [{}, {}]", static_cast<int>(m_player->position.x), static_cast<int>(m_player->position.y));

	SetConsoleCursorPos(0, (height + mainViewport->position.y + 2));
	std::cout << "\033[2K"; // Erase current line
	std::cout << std::format("Player Direction: [{}, {}]", m_player->direction.x, m_player->direction.y);

	// Run Raycaster
	Raycaster(mainViewport, m_player, mainCam, m_currentMap, m_textureList);

	DrawColorViewport(mainViewport);

	return 0;
}

void Game::Raycaster(Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*> textures)
{
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
	SpriteCasting(viewport, player, camera, map, zBuffer);

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
		if (wallData[static_cast<int>(plPosY) * mapSize.x + static_cast<int>(plPosX + plDirX * moveSpeed) ] == 0)
		{
			plPosX += plDirX * moveSpeed;
		}
		if (wallData[static_cast<int>(plPosY + plDirY * moveSpeed) * mapSize.x + static_cast<int>(plPosX) ] == 0)
		{
			plPosY += plDirY * moveSpeed;
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		// If x - direction or y - direction * movement speed  doesnt interact with wall
		if (wallData[static_cast<int>(plPosY) * mapSize.x + static_cast<int>(plPosX - plDirX * moveSpeed)] == 0)
		{
			plPosX -= plDirX * moveSpeed;
		}
		if (wallData[static_cast<int>(plPosY - plDirY * moveSpeed) * mapSize.x + static_cast<int>(plPosX)] == 0)
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

		float newCamSizeX = camera->size.x * cos(rotSpeed) - camera->size.y * sin(rotSpeed);
		float newCamSizeY = camera->size.x * sin(rotSpeed) + camera->size.y * cos(rotSpeed);

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

void Game::KeyboardInput(Player*& player, Map*& map)
{
	float const& plDirX = player->direction.x;
	float const& plDirY = player->direction.y;

	// Keyboard Inputs
	// Move Forward if not crash into wall
	if (player->IsMoving() == false)
	{
		if (GetAsyncKeyState(VK_UP))
		{
			player->PlayerMoveAttempt(Vector2{ plDirX, plDirY }, map);
			return;
		}
		if (GetAsyncKeyState(VK_DOWN))
		{
			player->PlayerMoveAttempt(Vector2{ -plDirX, -plDirY }, map);
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

void Game::CommandInput(string command, Player*& player, Map*& map)
{
	float const& plDirX = player->direction.x;
	float const& plDirY = player->direction.y;

	// Convert string to lowercase
	command = StringToLower(command);

	// Map Movement

	if (command == "move forward")
	{
		player->PlayerMoveAttempt(Vector2{ plDirX, plDirY }, map);
		return;
	}
	else if (command == "move backward")
	{
		player->PlayerMoveAttempt(Vector2{ -plDirX, -plDirY }, map);
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
		Room* room = GetRoomFromPos(m_currentRoom->GetPos() + Vector2i{ 0, 1 });

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
		Room* room = GetRoomFromPos(m_currentRoom->GetPos() + Vector2i{ 0, -1 });

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
		Room* room = GetRoomFromPos(m_currentRoom->GetPos() + Vector2i{ 1, 0 });

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
		Room* room = GetRoomFromPos(m_currentRoom->GetPos() + Vector2i{ -1, 0 });

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

void Game::CreateRoom(MapData& mapData)
{
	Room* room = new Room();

	Vector2i const& roomSize = mapData.mapSize;

	Map* map = new Map(roomSize);

	map->SetContents(mapData.mapData, roomSize);

	for (SpriteData const& sprite : mapData.sprites)
	{
		map->AddSprite(new Sprite{sprite.position, m_textureList[sprite.textureIndex], sprite.scale, sprite.offset});
	}

	for (int tex : mapData.wallTextures)
	{
		map->EmplaceLayerTexture(tex, MapDataType::WALL);
	}

	for (int tex : mapData.floorTextures)
	{
		map->EmplaceLayerTexture(tex, MapDataType::FLOOR);
	}

	for (int tex : mapData.roofTextures)
	{
		map->EmplaceLayerTexture(tex, MapDataType::ROOF);
	}

	room->SetMap(map);

	if (mapData.item != nullptr)
	{
		room->AddItem(mapData.item);
	}
	else
	{
		room->AddRandomItem();
	}

	room->SetStartingPosition(mapData.startingPostion);
	room->SetStartingDirection(mapData.startingDirection);

	if (!mapData.description.empty())
	{
		room->SetDescription(mapData.description);
	}

	room->SetRoomPosition(mapData.mapPosition);

	SetRoom(room);
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

void Game::SetRoom(Room* room) const
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
