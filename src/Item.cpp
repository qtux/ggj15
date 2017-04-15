/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "global.hpp"
#include "Item.hpp"
#include <cmath>
#include "Level.hpp"

Item::Item(sf::Sprite* sprite,int x,int y,int w,int h):
	mySprite(sprite),
	collectable(true),
	blocksPath(false),
	phaseOffset(rand()),
	waveClock(sf::Clock()),
	position(mySprite->getPosition())
{
	sprite->setScale(2,2);	// hack to allow 32x32 pixel per tile TODO replace sprites with rectangle and set size to pixelSize
	mySprite->setTextureRect(sf::IntRect(x, y, w, h));
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
	float t = waveClock.getElapsedTime().asSeconds();
	float offset = 6.0f * std::sin(0.75f * M_PI * t + phaseOffset);
	mySprite->setPosition(position.x, position.y + offset);
}

void Item::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	if (mySprite != 0)
	{
		renderTarget.draw(*mySprite, renderShader);
	}
}
