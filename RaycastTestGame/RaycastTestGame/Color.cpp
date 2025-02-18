#include "Color.h"

Color::Color()
{
}

Color::Color(byte r, byte g, byte b)
	: r{ r }, g{ g }, b{ b }
{

}

Color::~Color()
{
	delete[] m_ColorString;
}

char* Color::GetColorString()
{
	return m_ColorString;
}
