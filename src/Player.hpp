#pragma once

#include "GameObject.hpp"

class Player : public GameObject
{
public:
	int animationStep;
	int direction;

	Player(): direction(0) {};

	virtual void update(sf::Time deltaTime);
	
	
};

const static int PlayerAnimState[] = {0, 1, 0, 2};
const static int DoggieAnimState[] = {0,2,1,3,1,4};

const static int slowFactor = 10;
