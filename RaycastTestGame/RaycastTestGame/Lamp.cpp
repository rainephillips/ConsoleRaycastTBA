#include "Lamp.h"

#include <iostream>

Lamp::Lamp()
	: m_turnedOn{ false }
{
}

const void Lamp::Description()
{
	// Create output string
	string output;

	output.append("\033[2KThere is a lamp! The candle ");

	if (m_turnedOn) // If lamp is on
	{
		output.append("is burning bright! Would you like to destroy the flame?");
	}
	else
	{
		output.append("has fizzled out and is no longer burning... Would you like to light the lamp?");
	}

	std::cout << output;
}

void Lamp::Use()
{
	m_turnedOn = !m_turnedOn; // Invert m_turnedOn

	string output = string("\033[2KYou ");

	if (m_turnedOn) // If lamp is on
	{
		output.append("lit the flame... The light burns bright.");
	}
	else
	{
		output.append("extinguished the flame... The world feels empty...");
	}

	std::cout << output;
}
