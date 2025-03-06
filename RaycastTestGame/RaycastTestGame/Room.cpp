#include "Room.h"

#include <iostream>

#include "Item.h";
#include "Map.h";

Room::Room()
    : m_description{ string() }, m_map{ nullptr }
{
    m_items.clear();
    m_description = string("Too lazy to write description :(");
}

Room::Room(string decription, vector<Item*> items, Map* map)
    : m_description{ decription }, m_items{ items }, m_map{ map }
{
}

Room::~Room()
{
    if (m_map != nullptr)
    {
        delete m_map;
    }

    for (int item = 0; item < m_items.size(); item++)
    {
        delete m_items[item];
    }  

    m_items.clear();
}

void Room::SetMap(Map* map)
{
    if (m_map != nullptr)
    {
        delete m_map;
    }
    m_map = map;
}

void Room::Description()
{
    std::cout << "\033[2K" << m_description;
}

Vector2 Room::GetStartingPosition()
{
    return m_startingPosition;
}

Vector2 Room::GetStartingDirection()
{
    return m_startingDirection;
}

void Room::SetStartingPosition(Vector2 position)
{
    m_startingPosition = position;
}

void Room::SetStartingDirection(Vector2 direction)
{
    m_startingDirection = direction;
}

Map* Room::GetMap()
{
    return m_map;
}
