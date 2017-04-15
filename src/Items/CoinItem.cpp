/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "../global.hpp"
#include "CoinItem.hpp"
#include "../Level.hpp"

CoinItem::CoinItem(sf::Sprite* sprite):
	Item(sprite, 3*32, 2*32, 32, 32)
{}

void CoinItem::applyEffect(Level& level)
{
	gb::soundManager.playSound("sound/keyCollected.ogg");
	level.gui->addCoins(1);
	level.textBox->triggerText("coin");
}
