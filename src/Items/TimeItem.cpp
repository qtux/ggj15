#include "../global.hpp"
#include "TimeItem.hpp"
#include "../Level.hpp"

TimeItem::TimeItem(sf::Sprite* sprite):Item(sprite,0,48,16,16){
}

bool TimeItem::applyEffect(Level& level)
{
	gb::soundManager.playSound("sound/clockCollected.ogg");
	level.gui->applyTimeBufff(-1);
	return false;
};
