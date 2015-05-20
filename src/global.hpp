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
	
	// window properties
	static const int pixelSizeX = 32;
	static const int pixelSizeY = 32;
	static const int sizeX = 5;
	static const int sizeY = 4;
	static const int largeTileSizeX = 6;
	static const int largeTileSizeY = 6;
	static const int gridWidth = largeTileSizeX * sizeX * pixelSizeX;
	static const int gridHeight = largeTileSizeY * sizeY * pixelSizeY;
	static const int screenWidth = 800;
	static const int screenHeight = 600;
	
	// manager
	extern SoundManager soundManager;
	extern RessourceManager ressourceManager;
}
