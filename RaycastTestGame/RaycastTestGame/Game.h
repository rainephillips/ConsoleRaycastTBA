#pragma once

class Game
{
public:
	Game();
	int Run();

private:
	float time;
	float oldTime;

	bool gameIsRunning;
};

