#pragma once

#include "GameObject.hpp"

class Player : public GameObject
{
public:
	int animationStep;

	Player();

	virtual void update(sf::Time deltaTime);
	
	
};
