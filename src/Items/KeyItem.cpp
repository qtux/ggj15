/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "../global.hpp"
#include "KeyItem.hpp"
#include "../Level.hpp"

KeyItem::KeyItem(sf::Sprite* sprite):
	Item(sprite, 1*32, 2*32, 32, 32)
{}

void KeyItem::applyEffect(Level& level)
{
	gb::soundManager.playSound("sound/keyCollected.ogg");
	level.gui->addKeys(1);
	level.textBox->triggerText("key");
}
