#include "Player.h"

#include "Vector2.h"
#include "Camera.h"
#include "Map.h"

Player::Player()
	: position{ 0.f, 0.f }, direction{ -1.f, 0.f }, m_camera { new Camera() },
	m_movementSpeed{ 0.25f }, m_rotationSpeed{ 0.33f }
{
	AddSpells();
}

Player::Player(Vector2 position, Vector2 direction)
	: position{ position }, direction{ direction }, m_camera{ new Camera() },
	m_movementSpeed{ 0.25f }, m_rotationSpeed{ 0.33f }
{
	AddSpells();
}

Player::Player(Vector2 position, Vector2 direction, Vector2 cameraSize, Vector2i viewportPosition, Vector2i viewportSize)
	: position{ position }, direction{ direction }, 
	m_camera{ new Camera(cameraSize, viewportPosition, viewportSize ) },
	m_movementSpeed{ 0.25f }, m_rotationSpeed{ 0.33f }
{
	AddSpells();
}

Player::~Player()
{
	// For all remaining tweens delete them
	for (Tween<float>* tween : m_playerTweens)
	{
		delete tween;
	}

	delete m_camera;
}

void Player::RunTweens(float delta)
{
	// For each tween
	for (int tween = 0; tween < m_playerTweens.size(); tween++)
	{
		// If tween is finished already delete it and go back to now moved back tween
		if (m_playerTweens[tween]->IsFinished())
		{
			// Remove tween and restart for loop
			delete m_playerTweens[tween];
			m_playerTweens.erase(m_playerTweens.begin() + tween);
			tween--;
		}
		else
		{
			// Run tween
			m_playerTweens[tween]->RunTween(delta);

			// If tween allows next tweens to run along with it
			if (m_playerTweens[tween]->IsContinuous() == false)
			{
				// Finish tween loop
				return;
			}

			// Same as first if statement now checking if tween is done
			if (m_playerTweens[tween]->IsFinished())
			{
				// Remove tween and restart for loop
				delete m_playerTweens[tween];
				m_playerTweens.erase(m_playerTweens.begin() + tween);
				tween--;
			}
		}
	}
}

void Player::AddTween(Tween<float>* tween)
{
	m_playerTweens.emplace_back(tween);
}

Camera* Player::GetCamera()
{
	return m_camera;
}

void Player::AddSpells()
{
	m_spells.emplace_back("maziodyne");
	m_spells.emplace_back("myriad truths");
	m_spells.emplace_back("psi");
}

int Player::SearchForSpell(string spell)
{
	// BINARY SEARCH FOR SPELL STRING
	int lowIndex = 0;
	int highIndex = m_spells.size() - 1;

	int midpoint;
	int comparrisonResult;

	// WHILE THERE ARE MORE VALUES TO CHECK
	while (lowIndex <= highIndex)
	{
		// Assign midpoint
		midpoint = (lowIndex + highIndex) / 2;

		// Compare the 2 strings to see if before after or exact same in ascii order
		comparrisonResult = strcmp(spell.c_str(), m_spells[midpoint].c_str());

		switch (comparrisonResult)
		{
			// If identical found
			case 0:
			{
				return midpoint;
			}

			// If after cut array in half on rhs
			case 1:
			{
				lowIndex = midpoint + 1;
				break;
			}

			// If before cut array in half on rhs
			case -1:
			{
				highIndex = midpoint - 1;
				break;
			}

			default:
			{
				break;
			}
		}
	}

	return -1;
}

bool Player::IsMoving()
{
	return (m_playerTweens.size() > 0);
}

bool Player::CheckCollision(Vector2 position, Map*& map, bool xAxis)
{
	// Get wall data
	uint16_t* wallData = map->GetDataTypeBuffer(MapDataType::WALL);

	// Get map size
	Vector2i mapSize = map->GetMapSize();

	bool result;
	
	// If location is intesecting with a wall
	// Return true if not colliding
	if (xAxis)
	{
		result = (wallData[int(this->position.y) * mapSize.x + int(this->position.x + position.x)] == 0);
	}
	else
	{
		result = (wallData[int(this->position.y + position.y) * mapSize.x + int(this->position.x)] == 0);
	}

	delete[] wallData;

	return result;
}

void Player::PlayerMoveAttempt(Vector2 position, Map*& map)
{
	
	// If Colliding on x axis
	if (CheckCollision(position, map, true))
	{
		// Tween x movement
		AddTween(new Tween<float>(this->position.x, this->position.x + position.x, std::ref(this->position.x), m_movementSpeed, true));
	}
	else
	{
		// Tween ram into wall and bounce back
		AddTween(new Tween<float>(this->position.x, this->position.x + position.x * 0.45f, std::ref(this->position.x), m_movementSpeed / 2, false));
		AddTween(new Tween<float>(this->position.x + position.x * 0.45f, this->position.x, std::ref(this->position.x), m_movementSpeed / 2, true));
	}

	// If colliding on y axis
	if (CheckCollision(position, map, false))
	{
		// Tween y movement
		AddTween(new Tween<float>(this->position.y, this->position.y + position.y, std::ref(this->position.y), m_movementSpeed, true));
	}
	else
	{
		// Tween ram into wall and bounce back
		AddTween(new Tween<float>(this->position.y, this->position.y + position.y * 0.45f, std::ref(this->position.y), m_movementSpeed / 2, false));
		AddTween(new Tween<float>(this->position.y + position.y * 0.45f, this->position.y, std::ref(this->position.y), m_movementSpeed / 2, true));
	}
}

void Player::TurnPlayer(float rotation)
{
	float plNewDirX = direction.x * cos(rotation) - direction.y * sin(rotation);
	float plNewDirY = direction.x * sin(rotation) + direction.y * cos(rotation);

	float newCamSizeX = m_camera->size.x * cos(rotation) - m_camera->size.y * sin(rotation);
	float newCamSizeY = m_camera->size.x * sin(rotation) + m_camera->size.y * cos(rotation);

	// Rotate Player
	AddTween(new Tween<float>(direction.x, plNewDirX, std::ref(direction.x), m_rotationSpeed, true));
	AddTween(new Tween<float>(direction.y, plNewDirY, std::ref(direction.y), m_rotationSpeed, true));

	// Rotate Camera
	AddTween(new Tween<float>(m_camera->size.x, newCamSizeX, std::ref(m_camera->size.x), m_rotationSpeed, true));
	AddTween(new Tween<float>(m_camera->size.y, newCamSizeY, std::ref(m_camera->size.y), m_rotationSpeed, true));
}

float Player::GetMovementSpeed()
{
	return m_movementSpeed;
}

float Player::GetRotationSpeed()
{
	return m_rotationSpeed;
}

bool Player::FindSpell(string spell)
{
	return !(SearchForSpell(spell) == -1);
}

void Player::SetMovementSpeed(float speed)
{
	m_movementSpeed = speed;
}

void Player::SetRotationSpeed(float speed)
{
	m_rotationSpeed = speed;
}
