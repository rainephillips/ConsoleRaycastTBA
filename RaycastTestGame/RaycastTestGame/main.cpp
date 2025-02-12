#include <iostream>

#include "ConsoleUtils.h"

int main()
{
	setConsoleBufferResolution(128, 128);
	drawRect(1, 2, 1, 20, '#');
	drawRect(3, 2, 1, 20, '#');
	return EXIT_SUCCESS;
}