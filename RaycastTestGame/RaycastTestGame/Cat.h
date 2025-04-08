#pragma once
#include "Item.h"
class Cat : public Item
{
public:
	Cat();

public:
	void Description() const override;
	void Use() override;

private:
	bool m_purring;
};

