#pragma once
#include "Item.h"
class Lamp : public Item
{
public:
	Lamp();

public:
	void Description() const override;
	void Use() override;

private:
	bool m_turnedOn;
};

