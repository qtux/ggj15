#include "globals.hpp"
#include "KeyItem.hpp"

KeyItem::KeyItem(sf::Sprite* sprite):Item(sprite,0,32,16,16){
}

bool KeyItem::applyEffect()
{
	global::soundManager.playSound("sound/keyCollected.ogg");
	global::sceneManager.getCurrentScene().gui->addKeys(1);
	global::sceneManager.getCurrentScene().textBox->triggerText("key");
	return false;
};
