#pragma once

#include "Item.hpp"

class TriggerItem : public Item
{
public:
	TriggerItem(sf::Sprite* sprite);
	bool applyEffect();
	void setSwitchPos(int x1, int y1, int x2, int y2);

	sf::Vector2i switchPos1, switchPos2;
};
