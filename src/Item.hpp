#pragma once

#include "GameObject.hpp"

class Item : public GameObject
{
public:
	Item(sf::Sprite* sprite);
	
	virtual void update(sf::Time deltaTime);
	virtual void applyEffect();
};
