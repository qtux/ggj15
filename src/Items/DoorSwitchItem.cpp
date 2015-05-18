#include "DoorSwitchItem.hpp"
#include "../global.hpp"
#include "../Level.hpp"

DoorSwitchItem::DoorSwitchItem(sf::Sprite* sprite, bool toggled):
	Item(sprite, 0, 0, 0, 0),
	toggled(toggled)
{
	this->blocksPath = false;
	this->collectable = false;
	
	doorSwitchLeftTexture.height = 16;
	doorSwitchLeftTexture.width = 16;
	doorSwitchLeftTexture.left = 2*16;
	doorSwitchLeftTexture.top = 3*16;

	doorSwitchRightTexture.height = 16;
	doorSwitchRightTexture.width = 16;
	doorSwitchRightTexture.left = 3*16;
	doorSwitchRightTexture.top = 3*16;
}

// TODO make applyEffect always only once!
bool DoorSwitchItem::applyEffect(Level& level)
{
	toggled = !toggled;
	level.toggleDoors();
	return false;
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
			mySprite->setTextureRect(doorSwitchRightTexture);
		}
		else
		{
			mySprite->setTextureRect(doorSwitchLeftTexture);
		}
	}
}
