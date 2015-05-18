#pragma once

#include "../Item.hpp"

class Level;

class CoinItem : public Item
{
public:
	CoinItem(sf::Sprite* sprite);
	void applyEffect(Level& level);
};
