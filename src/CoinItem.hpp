#pragma once

#include "Item.hpp"

class CoinItem : public Item
{
public:
	CoinItem(sf::Sprite* sprite);
	void applyEffect();
};
