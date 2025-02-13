#include <iostream>

#include "Game.h"


int main()
{
	Game* newGame = new Game();

	int returnValue = newGame->Run();

	delete newGame;

	return returnValue;
}