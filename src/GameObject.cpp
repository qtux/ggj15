#include "GameObject.hpp"
#include <SFML/Graphics.hpp>

const sf::Vector2f& GameObject::getPosition() const
{
	return mySprite->getPosition();
}
	
void GameObject::setPosition(int x, int y)
{
	mySprite->setPosition(x, y);
}

void GameObject::setPosition(float x, float y)
{
	mySprite->setPosition(x, y);
}


int GameObject::getHeight() const
{
	const sf::IntRect& tmp = mySprite->getTextureRect();
	return int(tmp.height);
}

int GameObject::getWidth() const
{
	const sf::IntRect& tmp = mySprite->getTextureRect();
	return int(tmp.width);
}
