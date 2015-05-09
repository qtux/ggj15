/*
 * DoorItem.cpp
 *
 *  Created on: 28.02.2015
 *      Author: johannes
 */

#include "DoorItem.hpp"
#include "../global.hpp"
#include "../Level.hpp"

DoorItem::DoorItem(sf::Sprite* sprite, bool vertical_, bool closed):
	Item(sprite),
	wasClosed(closed),
	vertical(vertical_)
{
	this->blocksPath = closed;
	this->collectable = false;
	if (vertical)
	{
		openDoorTexture.height = 32;
		openDoorTexture.width = 16;
		openDoorTexture.left = 48;
		openDoorTexture.top = 64;

		closedDoorTexture.height = 32;
		closedDoorTexture.width = 16;
		closedDoorTexture.left = 32;
		closedDoorTexture.top = 64;
	}
	else
	{
		openDoorTexture.height = 16;
		openDoorTexture.width = 32;
		openDoorTexture.left = 32;
		openDoorTexture.top = 96;

		closedDoorTexture.height = 16;
		closedDoorTexture.width = 32;
		closedDoorTexture.left = 32;
		closedDoorTexture.top = 112;
	}
}

DoorItem::~DoorItem() {
}

bool DoorItem::applyEffect(Level& level) {
	return false;
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
			mySprite->setTextureRect(closedDoorTexture);
			wasClosed = true;
		}
		else
		{
			mySprite->setTextureRect(openDoorTexture);
			if (wasClosed) gb::soundManager.playSound("sound/door.ogg");
			wasClosed = false;
		}
	}
}

void DoorItem::toggle()
{
	blocksPath = !blocksPath;
}
