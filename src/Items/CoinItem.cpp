#include "../global.hpp"
#include "CoinItem.hpp"
#include "../Level.hpp"

CoinItem::CoinItem(sf::Sprite* sprite):
	Item(sprite,0,80,16,16)
{}

void CoinItem::applyEffect(Level& level)
{
	gb::soundManager.playSound("sound/keyCollected.ogg");
	level.gui->addCoins(1);
	level.textBox->triggerText("coin");
}
