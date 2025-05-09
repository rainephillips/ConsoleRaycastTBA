#include "Player.h"

#include "Camera.h"
#include "Map.h"

const float DEFAULT_ROTATION_SPEED = 0.33f;
const float DEFAULT_MOVEMENT_SPEED = 0.25f;

Player::Player()
	: position{ 0.f, 0.f }, direction{ -1.f, 0.f }, m_camera { new Camera() },
	m_movementSpeed{ DEFAULT_MOVEMENT_SPEED }, m_rotationSpeed{ DEFAULT_ROTATION_SPEED }
{
	AddSpells();
}

Player::Player(Vector2 position, Vector2 direction)
	: position{ position }, direction{ direction }, m_camera{ new Camera() },
	m_movementSpeed{ DEFAULT_MOVEMENT_SPEED }, m_rotationSpeed{ DEFAULT_ROTATION_SPEED }
{
	AddSpells();
}

Player::Player(Vector2 position, Vector2 direction, Vector2 cameraSize, Vector2i viewportPosition, Vector2i viewportSize)
	: position{ position }, direction{ direction }, 
	m_camera{ new Camera(cameraSize, viewportPosition, viewportSize ) },
	m_movementSpeed{ DEFAULT_MOVEMENT_SPEED }, m_rotationSpeed{ DEFAULT_ROTATION_SPEED }
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

int Player::SearchForSpell(string const & spell)
{
	// BINARY SEARCH FOR SPELL STRING
	size_t lowIndex = 0;
	size_t highIndex = m_spells.size() - 1;

	// WHILE THERE ARE MORE VALUES TO CHECK
	while (lowIndex <= highIndex)
	{
		// Assign midpoint
		size_t midpoint = (lowIndex + highIndex) / 2;

		// Compare the 2 strings to see if before after or exact same in ascii order
		switch (spell.compare(m_spells[midpoint]))
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

bool Player::IsMoving() const
{
	return (!m_playerTweens.empty());
}

bool Player::CheckCollision(Vector2 nextPosition, Map*& map, bool xAxis) const
{
	// Get wall data
	uint16_t* wallData = map->GetDataTypeBuffer(MapDataType::WALL);

	// Get map size
	Vector2i mapSize = map->GetMapSize();

	// If location is intesecting with a wall
	// Return true if not colliding
	bool result = xAxis ? (wallData[static_cast<int>(position.y) * mapSize.x + static_cast<int>(position.x + nextPosition.x)] == 0) :
							(wallData[static_cast<int>(position.y + nextPosition.y) * mapSize.x + static_cast<int>(position.x)] == 0);
	
	delete[] wallData;

	return result;
}

void Player::PlayerMoveAttempt(Vector2 nextPosition, Map*& map)
{
	
	// If Colliding on x axis
	if (CheckCollision(nextPosition, map, true))
	{
		// Tween x movement
		AddTween(new Tween<float>(position.x, position.x + nextPosition.x, std::ref(position.x), m_movementSpeed, true));
	}
	else
	{
		// Tween ram into wall and bounce back
		AddTween(new Tween<float>(position.x, position.x + nextPosition.x * 0.45f, std::ref(position.x), m_movementSpeed / 2, false));
		AddTween(new Tween<float>(position.x + nextPosition.x * 0.45f, position.x, std::ref(position.x), m_movementSpeed / 2, true));
	}

	// If colliding on y axis
	if (CheckCollision(nextPosition, map, false))
	{
		// Tween y movement
		AddTween(new Tween<float>(position.y, position.y + nextPosition.y, std::ref(position.y), m_movementSpeed, true));
	}
	else
	{
		// Tween ram into wall and bounce back
		AddTween(new Tween<float>(position.y, position.y + nextPosition.y * 0.45f, std::ref(position.y), m_movementSpeed / 2, false));
		AddTween(new Tween<float>(position.y + nextPosition.y * 0.45f, position.y, std::ref(position.y), m_movementSpeed / 2, true));
	}
}

void Player::TurnPlayer(float rotation)
{
	float plNewDirX = direction.x * cosf(rotation) - direction.y * sinf(rotation);
	float plNewDirY = direction.x * sinf(rotation) + direction.y * cosf(rotation);

	float newCamSizeX = m_camera->size.x * cosf(rotation) - m_camera->size.y * sinf(rotation);
	float newCamSizeY = m_camera->size.x * sinf(rotation) + m_camera->size.y * cosf(rotation);

	// Rotate Player
	AddTween(new Tween<float>(direction.x, plNewDirX, std::ref(direction.x), m_rotationSpeed, true));
	AddTween(new Tween<float>(direction.y, plNewDirY, std::ref(direction.y), m_rotationSpeed, true));

	// Rotate Camera
	AddTween(new Tween<float>(m_camera->size.x, newCamSizeX, std::ref(m_camera->size.x), m_rotationSpeed, true));
	AddTween(new Tween<float>(m_camera->size.y, newCamSizeY, std::ref(m_camera->size.y), m_rotationSpeed, true));
}

float Player::GetMovementSpeed() const
{
	return m_movementSpeed;
}

float Player::GetRotationSpeed() const
{
	return m_rotationSpeed;
}

bool Player::FindSpell(string const& spell)
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
