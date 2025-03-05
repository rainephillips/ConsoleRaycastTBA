#pragma once

#include <vector>
#include <string>
#include <thread>

class Camera;
class Map;
class Player;
class Texture;
class Viewport;
class Vector2i;

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

private:
	float m_deltaTime;
	float m_time;
	float m_oldTime;

	Player* m_player;
	Map* m_currentMap;

	vector<Texture*> m_textureList;

	bool m_gameIsRunning;
};

