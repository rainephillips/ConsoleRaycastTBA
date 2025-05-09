#include "Room.h"

#include <iostream>

#include "Item.h"

#include "BoxOfDonuts.h"
#include "Lamp.h"
#include "Cat.h"

#include "Map.h"

Room::Room()
    : m_item{ nullptr }, m_map{ nullptr }
{
    m_description = "Too lazy to write description :("; // Default descirption
}

Room::Room(string const& decription, Item* item, Map* map, Vector2i position)
    : m_item{ item }, m_description{ decription },  m_map{ map }, m_roomPosition{ position }
{
}

Room::~Room()
{
    if (m_map != nullptr)
    {
        delete m_map;
        m_map = nullptr;
    }

    if (m_item != nullptr)
    {
        delete m_item;
        m_item = nullptr;
    }
}

void Room::SetMap(Map* map)
{
    // If map exists
    if (m_map != nullptr)
    {
        delete m_map;
    }
    m_map = map;
}

void Room::Description() const
{
    std::cout << "\033[2K" << m_description;
}

Vector2 Room::GetStartingPosition() const
{
    return m_startingPosition;
}

Vector2 Room::GetStartingDirection() const
{
    return m_startingDirection;
}

Vector2i Room::GetPos() const
{
    return m_roomPosition;
}

Item* Room::GetItem() const
{
    return m_item;
}

void Room::SetStartingPosition(Vector2 position)
{
    m_startingPosition = position;
}

void Room::SetStartingDirection(Vector2 direction)
{
    m_startingDirection = direction;
}

void Room::SetDescription(string const& description)
{
    m_description = description;
}

void Room::SetRoomPosition(Vector2i position)
{
    m_roomPosition = position;
}

void Room::AddRandomItem()
{
    // Add random item
    switch (rand() % 4)
    {
        case 0:
        {
            AddItem(new Cat());
            break;
        }
        case 1:
        {
            AddItem(new Lamp());
            break;
        }
        case 2:
        {
            AddItem(new BoxOfDonuts());
            break;
        }

        default:
        {
            // Add no item
            break;
        }
    }
}

void Room::AddItem(Item* item)
{
    // If item items
    if (m_item != nullptr)
    {
        delete m_item;
    }

    m_item = item;
}

Map* Room::GetMap()
{
    return m_map;
}
