#include "Lamp.h"

#include <iostream>

Lamp::Lamp()
	: m_turnedOn{ false }
{
}

const void Lamp::Description()
{
	string output;

	output.append("\033[2KThere is a lamp! The candle ");

	if (m_turnedOn)
	{
		output.append("has fizzled out and is no longer burning... Would you like to light the lamp?");
	}
	else
	{
		output.append("is burning bright! Would you like to destroy the flame?");
	}
}

void Lamp::Use()
{
	m_turnedOn = !m_turnedOn;
}
