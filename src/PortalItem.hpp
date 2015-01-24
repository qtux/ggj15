#pragma once

#include "Item.hpp"

class PortalItem : public Item
{
public:
	PortalItem(sf::Sprite* sprite);
	void applyEffect();
	void update(sf::Time deltaTime);
};
