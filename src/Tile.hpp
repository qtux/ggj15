#pragma once

#include "GameObject.hpp"

class Tile : public GameObject
{
public:
	Tile();
	
	virtual void update(sf::Time deltaTime);
	const static int pixelSizeX = 8;
	const static int pixelSizeY = 8;
	const static int tileScaleFactor = 2;
	
};
