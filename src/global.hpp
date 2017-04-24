/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <bitset>
#include <string>
#include "SoundManager.hpp"
#include "RessourceManager.hpp"

#define PATH "./"
#define INPUT_SIZE 6

#ifdef FIX_TO_STRING
	#include <sstream>
	namespace std {
		std::string to_string(int value) {
			std::ostringstream ss;
			ss << value;
			return ss.str();
		}
	}
#endif

namespace gb {
	
	// window properties TODO move them into Level.cpp/Level.hpp
	static const sf::Vector2i tileSize(32, 32);
	static const sf::Vector2u gridSize(30, 24);
	static const sf::Vector2f sceneSize(gridSize.x * tileSize.x, gridSize.y * tileSize.y);
	
	// manager
	extern SoundManager soundManager;
	extern RessourceManager ressourceManager;
}
