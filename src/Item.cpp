#include "globals.hpp"
#include "Item.hpp"
#include <iostream>
#include <cmath>

Item::Item(sf::Sprite* sprite,int x,int y,int w,int h) : collectable(true), blocksPath(false){
	mySprite = sprite;
	drawX = x;
	drawY = y;
	drawW = w;
	drawH = h;
	waveClock = sf::Clock();
	waveClock.restart();
	offset = rand();
	mySprite->setTextureRect(sf::IntRect(drawX,drawY,drawW,drawH));
};
Item::~Item()
{
}


void Item::update (sf::Time deltaTime) {
	if (mySprite != 0)
	{
		mySprite->setTextureRect(sf::IntRect(drawX,drawY,drawW,drawH));
		sf::Vector2f vec = getPosition();
		setPosition(vec.x,vec.y+(std::sin((waveClock.getElapsedTime().asMilliseconds()+offset)/300)+0)*0.05);
		global::window.draw(*mySprite);
	}
};

bool Item::applyEffect()
{
	return false;
};
