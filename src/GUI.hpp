#pragma once

#include "GameObject.hpp"

class GUI : public GameObject
{
	GUI();

	virtual void update(sf::Time deltaTime);
};
