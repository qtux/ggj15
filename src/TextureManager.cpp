/*
 * TextureManager.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#include "TextureManager.hpp"
#include "globals.hpp"
#include <iostream>

TextureManager::TextureManager()
{
	tileTexture.loadFromFile(std::string(PATH) + "img/TileMap.png");
	itemsTexture.loadFromFile(std::string(PATH) + "img/items.png");
	playerTexture.loadFromFile(std::string(PATH) + "img/player.png");
	timeBarTexture.loadFromFile(std::string(PATH) + "img/timeBar.png");
	timeBarTexture.setRepeated(true);
}

TextureManager::~TextureManager() {
	// TODO Auto-generated destructor stub
}
