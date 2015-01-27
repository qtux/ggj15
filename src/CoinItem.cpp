#include "globals.hpp"
#include "CoinItem.hpp"

CoinItem::CoinItem(sf::Sprite* sprite):Item(sprite,0,80,16,16){
}

bool CoinItem::applyEffect()
{
	global::soundManager.playSound("sound/keyCollected.ogg");
	global::sceneManager.getCurrentScene().gui->addCoins(1);
	global::sceneManager.getCurrentScene().textBox->triggerText("coin");
	return false;
};
