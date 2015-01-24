#pragma once

#include "GameObject.hpp"

class Player : public GameObject
{
public:
	Player();

	virtual void update(sf::Time deltaTime);
	
	
};
