#include "globals.hpp"
#include "Tile.hpp"

Tile::Tile() {};


void Tile::update (sf::Time deltaTime) {
	if (mySprite != 0)
	{
		window.draw(*mySprite);
	}
};
