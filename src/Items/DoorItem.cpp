/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "DoorItem.hpp"
#include "../global.hpp"
#include "../Level.hpp"

DoorItem::DoorItem(sf::Sprite* sprite, bool vertical_, bool closed):
	Item(sprite, 0, 0 ,0, 0),
	wasClosed(closed),
	vertical(vertical_)
{
	blocksPath = closed;
	collectable = false;
	if (vertical)
	{
		openDoorTextureRect = sf::IntRect(48, 64, 16, 32);
		closedDoorTextureRect = sf::IntRect(32, 64, 16, 32);
	}
	else
	{
		openDoorTextureRect = sf::IntRect(32, 96, 32, 16);
		closedDoorTextureRect = sf::IntRect(32, 112, 32, 16);
	}
}

void DoorItem::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	if (mySprite != 0)
	{
		renderTarget.draw(*mySprite, renderShader);
	}
}

void DoorItem::update(sf::Time deltaTime) {
	//if (gb::sceneManager.getCurrentScene()->readyToLeave()) blocksPath = false; //TODO: temporäre Bedingung (deprecated) zum Testen, ersetzen!!

	if (mySprite != 0)
	{
		if (blocksPath)
		{
			mySprite->setTextureRect(closedDoorTextureRect);
			wasClosed = true;
		}
		else
		{
			mySprite->setTextureRect(openDoorTextureRect);
			if (wasClosed) gb::soundManager.playSound("sound/door.ogg");
			wasClosed = false;
		}
	}
}

void DoorItem::toggle()
{
	blocksPath = !blocksPath;
}
