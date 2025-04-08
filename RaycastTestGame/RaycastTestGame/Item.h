#pragma once

class Item
{
public:
	virtual ~Item();

public:
	virtual void Description() const;
	virtual void Use();
};

