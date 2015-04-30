#pragma once

#include "../Item.hpp"

class Level;

class KeyItem : public Item
{
public:
	KeyItem(sf::Sprite* sprite);
	bool applyEffect(Level& level);
};
