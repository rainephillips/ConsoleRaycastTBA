#include <iostream>
#include <cstdlib>

#include "Game.h"

/*

	WELCOME TO THE CONSOLE RENDERED TEXT BASED ADVENTURE

	IT IS RECCOMENDED YOU RESIZE THE CONSOLE BEFORE 
	RUNNING THE PROGRAM TO PREVENT PIXELS FROM REARRANGING

	COMMANDS (None are case sensitive)

	MAP MOVEMENT:

		- "Move Forward"	|	Moves player forward on the map.
		- "Move Backward"	|	Moves player backward on the map.
		- "Turn Left"		|	Turns the player left on the map.
		- "Turn Right"		|	Turns the player right on the map.

	ROOM MOVEMENT:

		- "Move Room North"	|	Move the player to the room north of current room if it exists.
		- "Move Room South"	|	Move the player to the room south of current room if it exists.
		- "Move Room East"	|	Move the player to the room east of current room if it exists.
		- "Move Room West"	|	Move the player to the room west of current room if it exists.

	MISC:

		- "Spell"			|	Asks the player to input a spell to search and returns whether the player knows the spell or not.
		- "Room Desc"		|	Returns the description of the room.
		- "Item Desc"		|	Returns the description of the item in the room if there is one.
		- "Item Interact"	|	Uses the item in the room if there is one.

	ESCAPE AND WINDOW PROPERTIES:

		- "Clear Screen"	|	Clears the console window do that you can resize the window and have the game properly display.
		- "Escape"			|	Exits the game.


*/

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
	//_CrtSetBreakAlloc(286);
	#endif 

	Game* newGame = new Game();

	int returnValue = newGame->Run();

	delete newGame;

	return returnValue;
}