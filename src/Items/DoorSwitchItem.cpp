#include "DoorSwitchItem.hpp"
#include "../global.hpp"
#include "../Level.hpp"

DoorSwitchItem::DoorSwitchItem(sf::Sprite* sprite, bool toggled):
	Item(sprite, 0, 0, 0, 0),
	toggled(toggled)
{
	blocksPath = false;
	collectable = false;
	doorSwitchLeftTextureRect = sf::IntRect(2*16, 3*16, 16, 16);
	doorSwitchRightTextureRect = sf::IntRect(3*16, 3*16, 16, 16);
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
