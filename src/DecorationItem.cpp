#include "globals.hpp"
#include "DecorationItem.hpp"

DecorationItem::DecorationItem(sf::Sprite* sprite, bool blocksPath_, int texPosX_, int texPosY_, int texW_, int texH_):Item(sprite,texPosX_,texPosY_,texW_,texH_) {
	this->blocksPath = blocksPath_;
	this->collectable = false;
}

void DecorationItem::applyEffect()
{
};
