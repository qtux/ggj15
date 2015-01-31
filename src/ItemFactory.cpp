#include "ItemFactory.hpp"
#include "TimeItem.hpp"
#include "CoinItem.hpp"
#include "TriggerItem.hpp"
#include "PortalItem.hpp"
#include "KeyItem.hpp"
#include "globals.hpp"
#include <SFML/Graphics.hpp>
#include "DecorationItem.hpp"
#include <iostream>


ItemFactory::ItemFactory() {
}

Item* ItemFactory::getItem(std::string name, bool blocksPath, int texPosX, int texPosY, int texW, int texH)
{
	sf::Sprite* sprite = new sf::Sprite();
	sprite->setTexture(gb::textureManager.itemsTexture);
	sprite->setPosition(10,gb::gridHeight-30);
	
	if (name == "TimeItem")
	{
		return new TimeItem(sprite);
	}
	if (name == "CoinItem")
	{
		return new CoinItem(sprite);
	}
	if (name == "TriggerItem")
	{
		return new TriggerItem(sprite);
	}
	if (name == "TriggerTrapItem")
	{
		sprite->setTextureRect(sf::IntRect(0,0,1,1));
		TriggerItem *tmp = new TriggerItem(sprite);
		tmp->drawX = 3*16;
		tmp->drawY = 6*16;
		tmp->drawW = 16;
		tmp->drawH = 16;
		tmp->mySprite->setTextureRect(sf::IntRect(tmp->drawX,tmp->drawY,tmp->drawW,tmp->drawH));
		return tmp;
	}
	if (name == "KeyItem")
	{
		return new KeyItem(sprite);
	}

	/*if (name == "TriggerItem")
	{
		return new TriggerItem(sprite);
	}*/
	if (name == "PortalItem")
	{
		return new PortalItem(sprite);
	}
	
	if (name == "DecorationItem")
	{
		return new DecorationItem(sprite, blocksPath, texPosX, texPosY, texW, texH);
	}
	return 0;
}
