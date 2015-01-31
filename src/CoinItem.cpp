#include "globals.hpp"
#include "CoinItem.hpp"

CoinItem::CoinItem(sf::Sprite* sprite):Item(sprite,0,80,16,16){
}

bool CoinItem::applyEffect()
{
	gb::soundManager.playSound("sound/keyCollected.ogg");
	gb::sceneManager.getCurrentScene().gui->addCoins(1);
	gb::sceneManager.getCurrentScene().textBox->triggerText("coin");
	return false;
};
