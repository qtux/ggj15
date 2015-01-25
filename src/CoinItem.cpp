#include "globals.hpp"
#include "CoinItem.hpp"

CoinItem::CoinItem(sf::Sprite* sprite):Item(sprite,0,80,16,16){
}

void CoinItem::applyEffect()
{
	soundManager.playSound("sound/keyCollected.ogg");
	sceneManager.getCurrentScene().gui->addCoins(1);
	sceneManager.getCurrentScene().textBox->triggerText("coin");
};
