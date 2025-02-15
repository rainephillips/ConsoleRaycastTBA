#pragma once

class Game
{
public:
	Game();
	int Run();

private:
	void Raycast();

private:
	float m_time;
	float m_oldTime;
	float deltaTime;

	bool gameIsRunning;
};

