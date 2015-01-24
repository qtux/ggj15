/*
 * TextureManager.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#include "TextureManager.hpp"
#include "Tile.hpp"
#include <iostream>

TextureManager::TextureManager() {
	// TODO Auto-generated constructor stub

}

TextureManager::~TextureManager() {
	// TODO Auto-generated destructor stub
}


void TextureManager::loadTexture(std::string name)
{
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile(name))
	{
		//std::cerr<<"could not load texture file "<<name<<std::endl;
		throw std::runtime_error("Texture file not found!");
	}
	bufferedTextures.insert(std::pair<std::string, sf::Texture *>(name, texture));
}

const sf::Texture &TextureManager::getTexture(std::string name)
{
	std::map<std::string, sf::Texture *>::const_iterator conIt = bufferedTextures.find(name);
	if (conIt == bufferedTextures.end())
	{
		loadTexture(name);
	}
	return *bufferedTextures[name];
}


void TextureManager::init(std::string preloadFileName)
{
	bufferedTextures.clear(); //TODO: doesn't tidy up!
	std::vector<std::string> loadFnList;
	readResourcesFileNameList(preloadFileName, loadFnList);
	for (std::vector<std::string>::iterator ldIt = loadFnList.begin(); ldIt != loadFnList.end(); ldIt++)
	{
		loadTexture(*ldIt);
	}
}
