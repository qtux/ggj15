#include "globals.hpp"
#include "KeyItem.hpp"

KeyItem::KeyItem(sf::Sprite* sprite):Item(sprite,0,32,16,16){
}

void KeyItem::applyEffect()
{
	soundManager.playSound("sound/keyCollected.ogg");
	sceneManager.getCurrentScene().gui->addKeys(1);
};
