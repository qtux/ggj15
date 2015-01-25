#pragma once
#include <string>
#include "Item.hpp"

class ItemFactory
{
public:
	ItemFactory();
	Item* getItem(std::string name, bool blocksPath = false, int texPosX = -1, int texPosY = -1, int texW = -1, int texH = -1);
};
