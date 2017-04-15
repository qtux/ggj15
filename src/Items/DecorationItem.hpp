/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include "../Item.hpp"
#include <map>

class Level;

class DecorationItem : public Item
{
public:
	DecorationItem(sf::Sprite* sprite, bool blocksPath, std::string type);
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	void update(sf::Time deltaTime);
	static const std::map<std::string, sf::Vector2i> typeMap;
};
