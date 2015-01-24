#pragma once

#include "Item.hpp"

class TriggerItem : public Item
{
public:
	TriggerItem(sf::Sprite* sprite);
	void applyEffect();
};
