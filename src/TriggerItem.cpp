#include "globals.hpp"
#include "TriggerItem.hpp"
#include <iostream>
#include "Tile.hpp"

TriggerItem::TriggerItem(sf::Sprite* sprite):Item(sprite,0,6*16,16,16){
}

void TriggerItem::applyEffect()
{
	sceneManager.getCurrentScene().switchLargeTile(switchPos1.x, switchPos1.y, switchPos2.x, switchPos2.y);
};

void TriggerItem::setSwitchPos(int x1, int y1, int x2, int y2)
{
	switchPos1.x = x1;
	switchPos1.y = y1;
	switchPos2.x = x2;
	switchPos2.y = y2;
}
