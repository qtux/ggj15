#include "globals.hpp";
#include "CoinItem.hpp";

CoinItem::CoinItem(sf::Sprite* sprite):Item(sprite,16,0,16,16){
}

void CoinItem::applyEffect()
{
	sceneManager.getCurrentScene().gui->addCoins(1);
};