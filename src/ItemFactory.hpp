#pragma once
#include <string>
#include "Item.hpp"

class ItemFactory
{
public:
	ItemFactory(sf::Sprite* sprite);
	Item* getItem(std::string name);
private:
	sf::Sprite* sprite;
};
