#include "ItemFactory.hpp"
#include "TimeItem.hpp"
#include "CoinItem.hpp"
#include "TriggerItem.hpp"
#include "PortalItem.hpp"
#include "KeyItem.hpp"
#include "globals.hpp"
#include <SFML/Graphics.hpp>


ItemFactory::ItemFactory() {
}

Item* ItemFactory::getItem(std::string name)
{
	sf::Sprite* sprite = new sf::Sprite();
	sprite->setTexture(textureManager.itemsTexture);
	sprite->setPosition(10,screenHeight-30);
	
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
	if (name == "KeyItem")
	{
		return new KeyItem(sprite);
	}

	if (name == "TriggerItem")
	{
		return new TriggerItem(sprite);
	}
	if (name == "PortalItem")
	{
		return new PortalItem(sprite);
	}
	return 0;
}
