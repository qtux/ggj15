#pragma once

#include "Item.hpp"

class KeyItem : public Item
{
public:
	KeyItem(sf::Sprite* sprite);
	bool applyEffect();
};
