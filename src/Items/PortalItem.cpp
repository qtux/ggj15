/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "../global.hpp"
#include "PortalItem.hpp"
#include <cmath>
#include "../Level.hpp"

PortalItem::PortalItem(sf::Sprite* sprite):
	Item(sprite,0,0,16,32),
	animClock(sf::Clock()),
	neverAgain(false)
{
	collectable = false;
}

void PortalItem::applyEffect(Level& level)
{
	if (level.readyToLeave() && !neverAgain) 
	{
		gb::soundManager.playSound("sound/won.ogg");
		neverAgain = true;
		level.leave();
	}
}

void PortalItem::update (sf::Time deltaTime)
{
	if (mySprite != nullptr)
	{
		int index = (1+std::sin(animClock.getElapsedTime().asSeconds() * 3.14)*2);
		mySprite->setTextureRect(sf::IntRect(16*index,0,16,32));
	}
}

void PortalItem::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	if (mySprite != nullptr)
	{
		renderTarget.draw(*mySprite);
	}
}
