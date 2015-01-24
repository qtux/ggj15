#pragma once

#include "GameObject.hpp"

class Tile : public GameObject
{
public:
	Tile();
	
	virtual void update(sf::Time deltaTime);
	const static int pixelSizeX = 16;
	const static int pixelSizeY = 16;
	
};
