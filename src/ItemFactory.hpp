#pragma once
#include <string>
#include "Item.hpp"

class ItemFactory
{
public:
	ItemFactory();
	Item* getItem(std::string name);
};
