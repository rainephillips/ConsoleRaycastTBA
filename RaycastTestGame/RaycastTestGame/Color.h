#pragma once

#include <string>

typedef unsigned char byte;

using std::string;

struct ColorA;

struct Color
{
public:
	Color();
	Color(unsigned int color);
	Color(byte r, byte g, byte b);

public:
	string ToStringValue(byte& colorValue);
	ColorA RGBToRGBA(Color& color);

public:
	byte r;
	byte g;
	byte b;

};

struct ColorA
{
public:
	ColorA();
	ColorA(unsigned int color);
	ColorA(byte r, byte g, byte b, byte a);

public:
	string ToStringValue(byte& colorValue);
	string ToANSIEscape();
	Color RGBAToRGB(ColorA& color);

public:
	byte r;
	byte g;
	byte b;
	byte a;
};
