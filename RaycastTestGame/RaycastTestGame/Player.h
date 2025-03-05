#pragma once

#include <vector>

#include "Camera.h"
#include "Tween.h";
#include "Vector2.h"

using std::vector;

class Map;

class Player
{
public:
	Player();
	Player(Vector2 position, Vector2 direction);
	Player(Vector2 position, Vector2 direction, Vector2 cameraSize, Vector2i viewportPosition, Vector2i viewportSize);
	~Player();

public:
	void RunTweens(float delta);
	void AddTween(Tween<float>* tween);

	bool IsMoving();
	bool CheckCollision(Vector2 position, Map*& map, bool xAxis);

	void PlayerMoveAttempt(Vector2 position, Map*& map);
	void TurnPlayer(float rotation);

	float GetMovementSpeed();
	float GetRotationSpeed();

	void SetMovementSpeed(float speed);
	void SetRotationSpeed(float speed);

	Camera* GetCamera();

public:
	Vector2 position;
	Vector2 direction;

private:
	Camera* m_camera;

	float m_movementSpeed;
	float m_rotationSpeed;

	vector<Tween<float>*> m_playerTweens;
};

