#include "globals.hpp"
#include "Tile.hpp"

Tile::Tile() : walkable(false) {};


void Tile::update (sf::Time deltaTime) {
	if (mySprite != 0)
	{
		global::window.draw(*mySprite);
	}
};
