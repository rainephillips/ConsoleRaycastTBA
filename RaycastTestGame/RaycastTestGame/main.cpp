#include <iostream>
#include <cstdlib>

#include "Game.h"

// _DEBUG is a macro that is automatically set when we are in debug builds
#if _DEBUG
// Define the macro that will enable the memory leak detection tools
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

int main()
{
	#if _DEBUG
	// These flags turn on the memory leak tracking tools
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// This function will enable a breakpoint when the program
	// reaches a piece of memoery with the matching ID
	//_CrtSetBreakAlloc(214425);
	#endif 

	Game* newGame = new Game();

	int returnValue = newGame->Run();

	delete newGame;

	return returnValue;
}