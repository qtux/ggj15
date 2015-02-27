#include "global.hpp"
#include "Tile.hpp"
#include <iostream>

Tile::Tile() : walkable(false) {};

bool Tile::intersects(const sf::Vector2f &testPos, const GameObject& cmp) const {return false;}


void Tile::update (sf::Time deltaTime) {
//	if (mySprite != 0)
//	{
//		gb::window.draw(*mySprite, &(gb::fragmentShader));
//	}
};

bool Tile::intersects(const GameObject& cmp) const {
	return false;
}

void Tile::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	if (mySprite != 0)
	{
		renderTarget.draw(*mySprite, renderShader);
	}
}
