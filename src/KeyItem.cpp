#include "globals.hpp"
#include "KeyItem.hpp"

KeyItem::KeyItem(sf::Sprite* sprite):Item(sprite,0,32,16,16){
}

bool KeyItem::applyEffect()
{
	gb::soundManager.playSound("sound/keyCollected.ogg");
	gb::sceneManager.getCurrentScene().gui->addKeys(1);
	gb::sceneManager.getCurrentScene().textBox->triggerText("key");
	return false;
};
