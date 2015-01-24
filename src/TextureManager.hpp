/*
 * TextureManager.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#ifndef TEXTUREMANAGER_HPP_
#define TEXTUREMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include "AbstractManager.hpp"

class TextureManager : public AbstractManager{
public:
	TextureManager();
	virtual ~TextureManager();

	const sf::Texture &getTexture(std::string name);
};

#endif /* TEXTUREMANAGER_HPP_ */
