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
