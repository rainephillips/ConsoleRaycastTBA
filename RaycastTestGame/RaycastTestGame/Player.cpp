#include "Player.h"

#include "Vector2.h"
#include "Camera.h"

Player::Player()
	: position{ 0.f, 0.f }, direction{ -1.f, 0.f }, m_camera { new Camera() }
{
}

Player::Player(Vector2 position, Vector2 direction)
	: position{ position }, direction{ direction }, m_camera{ new Camera() }
{
}

Player::Player(Vector2 position, Vector2 direction, Vector2 cameraSize, Vector2i viewportPosition, Vector2i viewportSize)
	: position{ position }, direction{ direction }, 
	m_camera{ new Camera(cameraSize, viewportPosition, viewportSize ) }
{
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

bool Player::IsMoving()
{
	return (m_playerTweens.size() > 0);
}
