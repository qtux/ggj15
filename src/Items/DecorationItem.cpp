/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "../global.hpp"
#include "DecorationItem.hpp"
#include "../Level.hpp"

const std::map<std::string, sf::Vector2i> DecorationItem::typeMap = {
		{"crystal_0", sf::Vector2i(0*32, 7*32)},
		{"crystal_1", sf::Vector2i(1*32, 6*32)},
		{"mushroom_0", sf::Vector2i(1*32, 5*32)},
		{"rock_0", sf::Vector2i(1*32, 7*32)},
	};

DecorationItem::DecorationItem(sf::Sprite* sprite, bool blocksPath_, std::string type):
	Item(sprite, typeMap.at(type).x, typeMap.at(type).y, 32, 32)
{
	blocksPath = blocksPath_;
	collectable = false;
}

void DecorationItem::update (sf::Time deltaTime) {}

void DecorationItem::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader)
{
	if (mySprite != nullptr)
	{
		renderTarget.draw(*mySprite);
	}
}
