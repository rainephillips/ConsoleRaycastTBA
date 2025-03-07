#include "BoxOfDonuts.h"

#include <iostream>
#include <format>

BoxOfDonuts::BoxOfDonuts()
	: m_count{ 6 }
{
}

const void BoxOfDonuts::Description()
{
	std::cout << std::format
	(
		"\033[2KIts a scrumdiddlyumptious box of donuts! There are {} donut(s) left! {}",
		m_count,
		(m_count > 0) ? "Delicious!" : "Oh no!" // If there are any donuts left
	);
}

void BoxOfDonuts::Use()
{
	if (m_count > 0) // If there are any donuts left
	{
		m_count--; // remove 1 donut

		std::cout << std::format
		(
			"\033[2KYou ate a donut! It did absolutely nothing! There are {} donuts remaining!",
			(m_count > 0) ? std::to_string(m_count) : "no" // If there are any donuts left
		);
	}
	else
	{
		std::cout << "\033[2KTheres no donuts left to eat... What's the point. ToT";
	}
}
