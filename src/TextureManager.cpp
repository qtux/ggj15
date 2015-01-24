/*
 * TextureManager.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#include "TextureManager.hpp"
#include <iostream>

TextureManager::TextureManager() {
	// TODO Auto-generated constructor stub

}

TextureManager::~TextureManager() {
	// TODO Auto-generated destructor stub
}

const sf::Texture &TextureManager::getTexture(std::string name)
{
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile(name))
	{
		std::cerr<<"could not load texture file "<<name<<std::endl;
	}

	return *texture;
}
