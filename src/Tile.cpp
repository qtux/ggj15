#include "globals.hpp"
#include "Tile.hpp"

Tile::Tile(){
	pixelSizeX = 16;
	pixelSizeY = 16;
};


void Tile::update (sf::Time deltaTime) {
	if (mySprite != 0)
	{
		window.draw(*mySprite);
	}
};
