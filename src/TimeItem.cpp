#include "globals.hpp"
#include "TimeItem.hpp"

TimeItem::TimeItem(sf::Sprite* sprite):Item(sprite,0,48,16,16){
}

void TimeItem::applyEffect()
{
	soundManager.playSound("sound/clockCollected.ogg");
	sceneManager.getCurrentScene().gui->applyTimeBufff(-20);
};
