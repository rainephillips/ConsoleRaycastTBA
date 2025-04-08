#pragma once
#include "Item.h"
class BoxOfDonuts : public Item
{
public:
	BoxOfDonuts();

public:
	void Description() const override;
	void Use() override;

private:
	int m_count;
};

