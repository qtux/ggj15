#include "ItemFactory.hpp"
#include "TimeItem.hpp"
#include "CoinItem.hpp"
#include "TriggerItem.hpp"
#include "KeyItem.hpp"

ItemFactory::ItemFactory(sf::Sprite* sprite) {
	this->sprite = sprite;
}

Item* ItemFactory::getItem(std::string name)
{
	if (name == "Time")
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
	return 0;
}