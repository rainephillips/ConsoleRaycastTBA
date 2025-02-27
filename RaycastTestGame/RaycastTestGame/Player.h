#pragma once

#include <vector>

#include "Camera.h"
#include "Tween.h";
#include "Vector2.h"

using std::vector;

class Player
{
public:
	Player();
	Player(Vector2 position, Vector2 direction);
	~Player();

public:
	void RunTweens(float delta);
	void AddTween(Tween<float>* tween);
	bool IsMoving();

public:
	Vector2 position;
	Vector2 direction;
	Camera* playerCam;

private:
	vector<Tween<float>*> m_playerTweens;
};

