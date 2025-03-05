#include "Room.h"

#include <iostream>

#include "Item.h";

Room::Room()
    : m_description{ string() }, m_map{ nullptr }
{
    m_description = string("Too lazy to write description :(");
}

Room::Room(string decription, vector<Item*> items, Map* map)
    : m_description{ decription }, m_items{ items }, m_map{ map }
{
}

Room::~Room()
{
    for (Item* item : m_items)
    {
        delete item;
    }

    delete m_map;
}

void Room::Description()
{
    std::cout << "\033[2K" << m_description;
}

Map* Room::GetMap()
{
    return m_map;
}
