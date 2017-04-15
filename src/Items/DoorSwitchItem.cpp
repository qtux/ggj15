/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "DoorSwitchItem.hpp"
#include "../global.hpp"
#include "../Level.hpp"

DoorSwitchItem::DoorSwitchItem(sf::Sprite* sprite, bool toggled):
	Item(sprite, 0, 0, 0, 0),
	toggled(toggled)
{
	blocksPath = false;
	collectable = false;
	int size = 32;
	doorSwitchLeftTextureRect = sf::IntRect(2*size, 3*size, size, size);
	doorSwitchRightTextureRect = sf::IntRect(3*size, 3*size, size, size);
}

// TODO make applyEffect always only once!
void DoorSwitchItem::applyEffect(Level& level)
{
	toggled = !toggled;
	level.toggleDoors();
}


void DoorSwitchItem::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	if (mySprite != 0)
	{
		renderTarget.draw(*mySprite, renderShader);
	}
}

void DoorSwitchItem::update(sf::Time deltaTime)
{
	if (mySprite != 0)
	{
		if (toggled)
		{
			mySprite->setTextureRect(doorSwitchRightTextureRect);
		}
		else
		{
			mySprite->setTextureRect(doorSwitchLeftTextureRect);
		}
	}
}
