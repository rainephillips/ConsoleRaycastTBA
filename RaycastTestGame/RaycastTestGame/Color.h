#pragma once

#include <string>

typedef unsigned char byte;

using std::string;

struct Color
{
public:
	Color();
	Color(byte r, byte g, byte b);

public:
	string ToStringValue(byte colorValue);

public:
	byte r;
	byte g;
	byte b;

};
