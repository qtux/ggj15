#include "globals.hpp"
#include "PortalItem.hpp"
#include <cmath>
PortalItem::PortalItem(sf::Sprite* sprite):Item(sprite,0,16,16,16){
}

void PortalItem::applyEffect()
{
	sceneManager.getCurrentScene().leave();
};

void PortalItem::update (sf::Time deltaTime) {
	if (mySprite != 0)
	{
		int index = (1+std::sin(deltaTime.asMilliseconds()/1000))/2;
		mySprite->setTextureRect(sf::IntRect(0,16*index,16,32));
		window.draw(*mySprite);
	}
};
