#include "globals.hpp"
#include "PortalItem.hpp"
#include <cmath>
#include <iostream>

PortalItem::PortalItem(sf::Sprite* sprite):Item(sprite,0,0,16,32){
	collectable = false;
	animClock = sf::Clock();
	animClock.restart();
}
PortalItem::~PortalItem(){
}

bool PortalItem::applyEffect()
{
	if (sceneManager.getCurrentScene().readyToLeave()) soundManager.playSound("sound/won.ogg");
	sceneManager.getCurrentScene().leave();
	return true;
};

void PortalItem::update (sf::Time deltaTime) {
	if (mySprite != 0)
	{
		int index = (1+std::sin(animClock.getElapsedTime().asMilliseconds()/300)*2);
		mySprite->setTextureRect(sf::IntRect(16*index,0,16,32));
		window.draw(*mySprite);
	}
};
