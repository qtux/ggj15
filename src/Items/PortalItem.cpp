#include "../global.hpp"
#include "PortalItem.hpp"
#include <cmath>
#include "../Level.hpp"

PortalItem::PortalItem(sf::Sprite* sprite):
	Item(sprite,0,0,16,32)
{
	neverAgain=false;
	collectable = false;
	animClock = sf::Clock();
	animClock.restart();
}

bool PortalItem::applyEffect(Level& level)
{
	if (level.readyToLeave() && !neverAgain) 
	{
		gb::soundManager.playSound("sound/won.ogg");
		neverAgain = true;
		level.leave();
		return true;
	}
	return false;
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
