#include "GameObject.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

const sf::Vector2f& GameObject::getPosition() const
{
	return mySprite->getPosition();
}

void GameObject::setPosition(float x, float y)
{
	mySprite->setPosition(x, y);
}

void GameObject::setPosition(const sf::Vector2f &a)
{
	mySprite->setPosition(a);
}

bool GameObject::intersects(const GameObject& cmp)
{
	return cmp.mySprite->getGlobalBounds().intersects(mySprite->getGlobalBounds());
}

int GameObject::getHeight() const
{
	const sf::IntRect& tmp = mySprite->getTextureRect();
	return int(tmp.height * mySprite->getScale().y);
}

int GameObject::getWidth() const
{
	const sf::IntRect& tmp = mySprite->getTextureRect();
	return int(tmp.width * mySprite->getScale().x);
}
