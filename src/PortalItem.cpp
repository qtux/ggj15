#include "globals.hpp"
#include "PortalItem.hpp"
#include <cmath>
PortalItem::PortalItem(sf::Sprite* sprite):Item(sprite,0,0,16,32){
	animClock = sf::Clock();
	animClock.restart();
}
PortalItem::~PortalItem(){
}

void PortalItem::applyEffect()
{
	sceneManager.getCurrentScene().leave();
};

void PortalItem::update (sf::Time deltaTime) {
	if (mySprite != 0)
	{
		int index = (1+std::sin(animClock.getElapsedTime().asMilliseconds()/300)*2);
		mySprite->setTextureRect(sf::IntRect(16*index,0,16,32));
		window.draw(*mySprite);
	}
};
