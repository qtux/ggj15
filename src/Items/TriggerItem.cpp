#include "../global.hpp"
#include "TriggerItem.hpp"
#include "../Level.hpp"

TriggerItem::TriggerItem(sf::Sprite* sprite):
	Item(sprite,0,6*16,16,16)
{}

void TriggerItem::applyEffect(Level& level)
{
	gb::soundManager.playSound("sound/switch.ogg");
	level.switchLargeTile(switchPos1, switchPos2);
	level.textBox->triggerText("trigger");
}

void TriggerItem::setSwitchPos(int x1, int y1, int x2, int y2)
{
	switchPos1.x = x1;
	switchPos1.y = y1;
	switchPos2.x = x2;
	switchPos2.y = y2;
}
