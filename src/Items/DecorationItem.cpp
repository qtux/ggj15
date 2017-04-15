/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "../global.hpp"
#include "DecorationItem.hpp"
#include "../Level.hpp"

DecorationItem::DecorationItem(sf::Sprite* sprite, bool blocksPath_, int texPosX_, int texPosY_, int texW_, int texH_):
	Item(sprite, texPosX_, texPosY_, texW_, texH_)
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
