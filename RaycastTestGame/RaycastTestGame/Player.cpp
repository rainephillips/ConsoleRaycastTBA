#include "Player.h"

#include "Vector2.h"
#include "Camera.h"

Player::Player()
	: position{ 0.f, 0.f }, direction{ -1.f, 0.f }
{
}

Player::Player(Vector2 position, Vector2 direction)
	: position{ position }, direction{ direction }
{
}

Player::~Player()
{
	for (Tween<float>* tween : m_playerTweens)
	{
		delete tween;
	}
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
			break;
		}

		m_playerTweens[tween]->RunTween(delta);

		if (m_playerTweens[tween]->IsContinuous() == false)
		{
			return;
		}

	}
}

void Player::AddTween(Tween<float>* tween)
{
	m_playerTweens.emplace_back(tween);
}

bool Player::IsMoving()
{
	return (m_playerTweens.size() > 0);
}
