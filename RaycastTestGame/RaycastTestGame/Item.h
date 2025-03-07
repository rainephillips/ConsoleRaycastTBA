#pragma once

#include <string>;

using std::string;

class Item
{
public:
	virtual const void Description();
	virtual void Use();
};

