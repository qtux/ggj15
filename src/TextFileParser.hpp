/*
 * TextFileparser.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#pragma once

#include <string>
#include "Scene.hpp"

class TextFileParser {
public:
	TextFileParser();
	virtual ~TextFileParser();
	static void loadTextFile(Scene &scene, std::string fileName);
};
