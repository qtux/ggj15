#pragma once

#include "../Item.hpp"

class Level;

class TriggerItem : public Item
{
public:
	TriggerItem(sf::Sprite* sprite, sf::Vector2u switchPos1, sf::Vector2u switchPos2);
	void applyEffect(Level& level);
	sf::Vector2u switchPos1, switchPos2;
};
