#pragma once

#include <string>
#include <vector>

#include "Vector2.h";

class Map;
class Item;

using std::string;
using std::vector;

class Room
{
public:
	Room();
	Room(string decription, vector<Item*> items, Map* map);
	~Room();

public:
	void SetMap(Map* map);
	void Description();

	Vector2 GetStartingPosition();
	Vector2 GetStartingDirection();

	void SetStartingPosition(Vector2 position);
	void SetStartingDirection(Vector2 direction);

	Map* GetMap();

private:
	vector<Item*> m_items;
	string m_description;
	Map* m_map;

	Vector2 m_startingPosition;
	Vector2 m_startingDirection;

};

