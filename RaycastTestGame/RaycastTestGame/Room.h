#pragma once

#include <string>
#include <vector>;

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
	void Description();
	Map* GetMap();

private:
	vector<Item*> m_items;
	string m_description;
	Map* m_map;
};

