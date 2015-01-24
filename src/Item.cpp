#include "globals.hpp"
#include "Item.hpp"

Item::Item(sf::Sprite* sprite){
	mySprite = sprite;
};


void Item::update (sf::Time deltaTime) {
	if (mySprite != 0)
	{
		window.draw(*mySprite);
	}
};

void Item::applyEffect()
{
		
};