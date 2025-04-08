#pragma once

#include <vector>
#include <string>

#include "Tween.h"
#include "Vector2.h"

using std::vector;
using std::string;

class Map;
class Camera;

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
	bool CheckCollision(Vector2 position, Map*& map, bool xAxis) const;

	void PlayerMoveAttempt(Vector2 position, Map*& map);
	void TurnPlayer(float rotation);

	float GetMovementSpeed() const;
	float GetRotationSpeed() const;

	bool FindSpell(string spell);

	void SetMovementSpeed(float speed);
	void SetRotationSpeed(float speed);

	Camera* GetCamera();

public:
	Vector2 position;
	Vector2 direction;


private:
	void AddSpells();

	int SearchForSpell(string spell);

private:
	Camera* m_camera;

	float m_movementSpeed;
	float m_rotationSpeed;

	vector<string> m_spells;

	vector<Tween<float>*> m_playerTweens;
};

