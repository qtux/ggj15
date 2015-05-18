#pragma once

#include "../Item.hpp"

class Level;

class TimeItem : public Item
{
public:
	TimeItem(sf::Sprite* sprite);
	void applyEffect(Level& level);
};
