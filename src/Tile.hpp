#pragma once

#include "GameObject.hpp"

class Tile : public GameObject
{
public:
	Tile();
	
	virtual void update(sf::Time deltaTime);
	
};
