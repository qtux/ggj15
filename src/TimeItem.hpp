#pragma once

#include "Item.hpp"

class TimeItem : public Item
{
public:
	TimeItem(sf::Sprite* sprite);
	void applyEffect();
};
