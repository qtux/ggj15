/*
 * TextFileParser.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#include "TextFileParser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

TextFileParser::TextFileParser() {
	// TODO Auto-generated constructor stub

}

TextFileParser::~TextFileParser() {
	// TODO Auto-generated destructor stub
}

void TextFileParser::loadTextFile(Scene &scene, std::string fileName)
{
	std::ifstream infile(fileName);
//	infile.open(fileName.c_str());
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string first;
		iss >> first;

		if (first == "start")
		{
			std::cout<<"start found"<<std::endl;
			int x,y;
			iss >> x;
			iss >> y;
			scene.startPos.x = x;
			scene.startPos.y = y;
		}

		if (first == "portal")
		{
			std::cout<<"portal found"<<std::endl;
			int x,y;
			iss >> x;
			iss >> y;
			scene.portalPos.x = x;
			scene.portalPos.y = y;
		}

//		std::cout<<line<<std::endl;

		// process pair (a,b)
	}
}
