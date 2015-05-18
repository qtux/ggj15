#include "global.hpp"
#include "Item.hpp"
#include <cmath>
#include "Level.hpp"

Item::Item(sf::Sprite* sprite,int x,int y,int w,int h) : collectable(true), blocksPath(false){
	mySprite = sprite;
	sprite->setScale(2,2);	// hack to allow 32x32 pixel per tile TODO replace sprites with rectangle and set size to pixelSize
	drawX = x;
	drawY = y;
	drawW = w;
	drawH = h;
	waveClock = sf::Clock();
	waveClock.restart();
	offset = rand();
	mySprite->setTextureRect(sf::IntRect(drawX,drawY,drawW,drawH));
};

Item::Item(sf::Sprite* sprite) : collectable(true), blocksPath(false) {
	mySprite = sprite;
}

Item::~Item()
{
}

const sf::Vector2f& Item::getPosition() const
{
	return mySprite->getPosition();
}

void Item::setPosition(float x, float y)
{
	mySprite->setPosition(x, y);
}

void Item::update (sf::Time deltaTime) {
//	if (mySprite != 0)
//	{
		mySprite->setTextureRect(sf::IntRect(drawX,drawY,drawW,drawH));
		sf::Vector2f vec = getPosition();
		setPosition(vec.x,vec.y+(std::sin((waveClock.getElapsedTime().asSeconds() * 1000 + offset)/300)+0)*0.05);
//		gb::window.draw(*mySprite);
//	}
};

void Item::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	if (mySprite != 0)
	{
		renderTarget.draw(*mySprite, renderShader);
	}
}

bool Item::applyEffect(Level& level)
{
	return false;
}

