#include "../global.hpp"
#include "KeyItem.hpp"
#include "../Level.hpp"

KeyItem::KeyItem(sf::Sprite* sprite):
	Item(sprite,0,32,16,16)
{}

bool KeyItem::applyEffect(Level& level)
{
	gb::soundManager.playSound("sound/keyCollected.ogg");
	level.gui->addKeys(1);
	level.textBox->triggerText("key");
	return false;
}
