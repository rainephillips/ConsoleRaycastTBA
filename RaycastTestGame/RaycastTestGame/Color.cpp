#include "Color.h"

Color::Color()
	: r{ 255 }, g{ 0 }, b{ 255 }
{
}

Color::Color(byte r, byte g, byte b)
	: r{ r }, g{ g }, b{ b }
{

}

string Color::ToStringValue(byte& colorValue)
{
	return string(std::to_string(short(colorValue)));
}

ColorA Color::RGBToRGBA(Color& color)
{
	return ColorA(r, g, b, 0);
}

ColorA::ColorA()
	:r{ 0 }, g{ 0 }, b{ 0 }, a{ 0 }
{
}

ColorA::ColorA(byte r, byte g, byte b, byte a)
	:r{ r }, g{ g }, b{ b }, a{ a }
{
}

string ColorA::ToStringValue(byte& colorValue)
{
	return string(std::to_string(short(colorValue)));
}

Color ColorA::RGBAToRGB(ColorA& color)
{
	return Color(r, g, b);
}
