/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "../global.hpp"
#include "TimeItem.hpp"
#include "../Level.hpp"

TimeItem::TimeItem(sf::Sprite* sprite):
	Item(sprite, 0, 3*32, 32, 32)
{}

void TimeItem::applyEffect(Level& level)
{
	gb::soundManager.playSound("sound/clockCollected.ogg");
	level.gui->applyTimeBufff(-1);
}
