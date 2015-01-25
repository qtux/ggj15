#pragma once

#include "Item.hpp"

class PortalItem : public Item
{
public:
	PortalItem(sf::Sprite* sprite);
	virtual ~PortalItem();
	void applyEffect();
	virtual void update(sf::Time deltaTime);
};
