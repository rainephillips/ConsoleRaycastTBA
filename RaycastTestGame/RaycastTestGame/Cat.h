#pragma once
#include "Item.h"
class Cat : public Item
{
public:
	Cat();

public:
	const void Description() override;
	void Use() override;

private:
	bool m_purring;
};

