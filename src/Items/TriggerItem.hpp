/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

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
