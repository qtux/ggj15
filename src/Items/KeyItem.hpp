/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include "../Item.hpp"

class Level;

class KeyItem : public Item
{
public:
	KeyItem(sf::Sprite* sprite);
	void applyEffect(Level& level);
};
