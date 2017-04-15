/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "../global.hpp"
#include "TriggerItem.hpp"
#include "../Level.hpp"

TriggerItem::TriggerItem(sf::Sprite* sprite, sf::Vector2u switchPos1, sf::Vector2u switchPos2):
	Item(sprite,0,6*16,16,16),
	switchPos1(switchPos1),
	switchPos2(switchPos2)
{}

void TriggerItem::applyEffect(Level& level)
{
	gb::soundManager.playSound("sound/switch.ogg");
	level.switchLargeTile(switchPos1, switchPos2);
	level.textBox->triggerText("trigger");
}
