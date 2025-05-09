#pragma once

#include <vector>
#include <string>
#include <thread>

#include "MapData.h"

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


	void Raycaster(Viewport*& viewport, Player*& player, Camera*& camera, Map*& map, vector<Texture*> textures);
	void OldKeyboardInput(Player*& player, Camera*& camera, Map*& map, float deltaTime);
	void KeyboardInput(Player*& player, Map*& map);
	void CommandInput(string command, Player*& player, Map*& map);
	void CreateRoom(MapData& mapData);

	void ChangeRoom(Room* room);
	Room* GetRoomFromPos(Vector2i position);

	void SetRoom(Room* room) const;

private:	
	float m_deltaTime;
	float m_oldTime;
	
	float m_playerFOV;
	Vector2i m_screenBufferResolution;
	Vector2i m_mainViewportOffset;
	Vector2i m_viewportResolution;
	Vector2i m_beginnerRoomPos;

	Room*** m_rooms;

	Vector2i m_roomsSize;
	Room* m_currentRoom;

	Player* m_player;
	Map* m_currentMap;

	vector<string> m_texturePaths;
	vector<Texture*> m_textureList;

	vector<MapData> m_mapData;

	bool m_gameIsRunning;

	bool m_useCommandInput;
};

