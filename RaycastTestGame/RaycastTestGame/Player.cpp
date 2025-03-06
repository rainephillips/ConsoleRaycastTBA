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
	for (Tween<float>* tween : m_playerTweens)
	{
		delete tween;
	}

	delete m_camera;
}

void Player::RunTweens(float delta)
{
	for (int tween = 0; tween < m_playerTweens.size(); tween++)
	{
		if (m_playerTweens[tween]->IsFinished())
		{
			// Remove tween and restart for loop
			delete m_playerTweens[tween];
			m_playerTweens.erase(m_playerTweens.begin() + tween);
			tween--;
		}
		else
		{
			m_playerTweens[tween]->RunTween(delta);

			if (m_playerTweens[tween]->IsContinuous() == false)
			{
				return;
			}

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
	m_spells.emplace_back("Maziodyne");
	m_spells.emplace_back("Myriad Truths");
	m_spells.emplace_back("Psi");
}

void Player::SearchForSpell(string spell)
{
}

bool Player::IsMoving()
{
	return (m_playerTweens.size() > 0);
}

bool Player::CheckCollision(Vector2 position, Map*& map, bool xAxis)
{
	uint16_t* wallData = map->GetDataTypeBuffer(MapDataType::WALL);

	Vector2i mapSize = map->GetMapSize();

	bool result;
	
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

	// Change to if x if y else pushback

	if (CheckCollision(position, map, true))
	{
		AddTween(new Tween<float>(this->position.x, this->position.x + position.x, std::ref(this->position.x), m_movementSpeed, true));
	}
	else
	{
		AddTween(new Tween<float>(this->position.x, this->position.x + position.x * 0.45f, std::ref(this->position.x), m_movementSpeed / 2, false));
		AddTween(new Tween<float>(this->position.x + position.x * 0.45f, this->position.x, std::ref(this->position.x), m_movementSpeed / 2, true));
	}

	if (CheckCollision(position, map, false))
	{
		AddTween(new Tween<float>(this->position.y, this->position.y + position.y, std::ref(this->position.y), m_movementSpeed, true));
	}
	else
	{
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

bool Player::HasSpell(string spell)
{
	return false;
}

void Player::SetMovementSpeed(float speed)
{
	m_movementSpeed = speed;
}

void Player::SetRotationSpeed(float speed)
{
	m_rotationSpeed = speed;
}
