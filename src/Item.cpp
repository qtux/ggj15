#include "globals.hpp"
#include "Item.hpp"
#include <iostream>

Item::Item(sf::Sprite* sprite,int x,int y,int w,int h){
	mySprite = sprite;
	drawX = x;
	drawY = y;
	drawW = w;
	drawH = h;
};


void Item::update (sf::Time deltaTime) {
	if (mySprite != 0)
	{
		mySprite->setTextureRect(sf::IntRect(drawX,drawY,drawW,drawH));
		window.draw(*mySprite);
	}
};

void Item::applyEffect()
{
		
};
