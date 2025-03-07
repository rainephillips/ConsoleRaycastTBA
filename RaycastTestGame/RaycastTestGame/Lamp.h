#pragma once
#include "Item.h"
class Lamp : public Item
{
public:
	Lamp();

public:
	const void Description() override;
	void Use() override;

private:
	bool m_turnedOn;
};

