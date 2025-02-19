#include "Color.h"

Color::Color()
	: r{ 255 }, g{ 0 }, b{ 255 }
{
}

Color::Color(byte r, byte g, byte b)
	: r{ r }, g{ g }, b{ b }
{

}

string Color::ToStringValue(byte colorValue)
{
	return string(std::to_string(short(colorValue)));
}
