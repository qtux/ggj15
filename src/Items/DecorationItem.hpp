/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include "../Item.hpp"

class Level;

class DecorationItem : public Item
{
public:
	DecorationItem(sf::Sprite* sprite, bool blocksPath, int texPosX, int texPosY, int texW, int texH);
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	void update(sf::Time deltaTime);
};
