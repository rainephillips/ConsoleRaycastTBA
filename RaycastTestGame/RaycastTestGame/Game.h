#pragma once

#include <vector>
#include <string>
#include <thread>

#include "Vector2.h"

class Camera;
class Map;
class Player;
class Texture;
class Room;
class Viewport;

using std::string;
using std::thread;
using std::vector;

class Game
{
public:
	Game();

public:
	int Run();

private:

	int BeginPlay();
	int EndPlay();

	int Tick(float deltaTime);


	void Raycaster(Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*> textures, bool useASCII);
	void OldKeyboardInput(Player*& player, Camera*& camera, Map*& map, float deltaTime);
	void KeyboardInput(Player*& player, Camera*& camera, Map*& map);
	void CommandInput(string command, Player*& player, Camera*& camera, Map*& map);
	void CreateDefaultTextures(vector<Texture*>& textureList, Vector2i textureSize);

	void ChangeRoom(Room* room);
	Room* GetRoomFromPos(Vector2i position);

	void SetRoom(Room* room);

private:	
	float m_deltaTime;
	float m_time;
	float m_oldTime;
	
	Room*** m_rooms;

	Vector2i m_roomsSize;
	Room* m_currentRoom;

	Player* m_player;
	Map* m_currentMap;

	vector<Texture*> m_textureList;

	bool m_gameIsRunning;
};

