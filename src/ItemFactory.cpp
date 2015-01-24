#include "ItemFactory.hpp"
#include "TimeItem.hpp"
#include "CoinItem.hpp"

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
	return 0;
}