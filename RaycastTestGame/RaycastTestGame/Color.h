#pragma once

typedef unsigned char byte;

struct Color
{
public:
	Color();
	Color(byte r, byte g, byte b);
	~Color();

public:
	char* GetColorString();

public:
	byte r;
	byte g;
	byte b;

private:
	char* m_ColorString;
};
