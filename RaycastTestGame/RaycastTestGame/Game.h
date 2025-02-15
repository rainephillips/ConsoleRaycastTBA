#pragma once

class Game
{
public:
	Game();
	int Run();

public:
	float deltaTime;

private:
	void Raycast();

private:
	float m_time;
	float m_oldTime;

	bool gameIsRunning;
};

