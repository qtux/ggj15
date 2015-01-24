/*
 * TextureManager.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#pragma once

#include <SFML/Graphics.hpp>

class TextureManager {
public:
	TextureManager();
	virtual ~TextureManager();

	sf::Texture tileTexture;
	sf::Texture itemsTexture;
	sf::Texture playerTexture;
	sf::Texture timeBarTexture;
};
