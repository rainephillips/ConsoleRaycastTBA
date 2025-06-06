#include "Cat.h"

#include <iostream>
#include <string>

using std::string;

Cat::Cat()
	: m_purring{ false }
{
}

void Cat::Description() const
{
	// Create output string

	string output = "It's a tuxedo cat! ";

	if (m_purring) // If cat is purring
	{
		output.append("They are happily purring rigt now! ");
	}
	else
	{
		output.append("They don't seem to pleased right now... ");
	}

	output.append("Would you like to pet them?");

	std::cout << output;
}

void Cat::Use()
{
	m_purring = !m_purring; // Invert m_purring

	if (m_purring) // If purring
	{
		std::cout << "\033[2K\"Mwhehehe mrow!\" The cat is loving it!";
	}
	else
	{
		std::cout << "\033[2K\"Hey! Don't pet me! I am not a cat!\" The \"cat\" clearly didn't like that...";
	}
}
