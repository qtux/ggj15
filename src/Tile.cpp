#include "global.hpp"
#include "Tile.hpp"
#include <iostream>

Tile::Tile() : walkable(false) {};


void Tile::update (sf::Time deltaTime) {
//	if (mySprite != 0)
//	{
//		gb::window.draw(*mySprite, &(gb::fragmentShader));
//	}
};

void Tile::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	if (mySprite != 0)
	{
		renderTarget.draw(*mySprite, renderShader);
	}
}
