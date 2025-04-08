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
	Room(string decription, Item* item, Map* map, Vector2i position);
	~Room();

public:
	void SetMap(Map* map);
	void Description() const;

	Vector2 GetStartingPosition() const;
	Vector2 GetStartingDirection() const;
	Vector2i GetPos() const;
	Item* GetItem() const;

	void SetStartingPosition(Vector2 position);
	void SetStartingDirection(Vector2 direction);
	void SetDescription(string description);
	void SetRoomPosition(Vector2i position);
	
	void AddRandomItem();

	void AddItem(Item* item);

	Map* GetMap();

private:
	Item* m_item;
	string m_description;
	Map* m_map;

	Vector2i m_roomPosition;

	Vector2 m_startingPosition;
	Vector2 m_startingDirection;

};

