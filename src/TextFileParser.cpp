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
#include "CoinItem.hpp"
#include "TimeItem.hpp"
#include "TriggerItem.hpp"
#include "KeyItem.hpp"
#include "ItemFactory.hpp"
#include "globals.hpp"
#include "TextureManager.hpp"
#include "Tile.hpp"
#include "TextElement.hpp"

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

//	sf::Texture *itemsTexture;
//	itemsTexture->loadFromFile(std::string(PATH) + "img/items.png");
	sf::Sprite *itemSprite = new sf::Sprite();
	itemSprite->setTexture(textureManager.itemsTexture);
	ItemFactory tmpFactory = ItemFactory();

	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string first;
		iss >> first;

		if (first == "Start")
		{
			int x,y;
			iss >> x;
			iss >> y;
			scene.startPos.x = x * pixelSizeX;
			scene.startPos.y = y * pixelSizeY;
			scene.player->setPosition(scene.startPos.x, scene.startPos.y);
			scene.player->doggieSprite->setPosition(scene.startPos.x, scene.startPos.y);
		}

		if (first == "Portal")
		{
			int x,y;
			iss >> x;
			iss >> y;
			scene.portalPos.x = x * pixelSizeX;
			scene.portalPos.y = y * pixelSizeY;
			Item *tmpItem = tmpFactory.getItem("PortalItem");
			tmpItem->setPosition(x * pixelSizeX, y * pixelSizeY);
			scene.items.push_back(tmpItem);
		}

		if (first == "Item")
		{
			std::string second;
			iss >> second;
			int x,y;
			iss >> x;
			iss >> y;
			
			Item *tmpItem = 0;
			if (second == "DecorationItem")
			{
				bool blocksPath;
				int texPosX, texPosY, texW, texH;
				iss >> blocksPath;
				iss >> texPosX;
				iss >> texPosY;
				iss >> texW;
				iss >> texH;
				tmpItem = tmpFactory.getItem(second, blocksPath, texPosX, texPosY, texW, texH);
			}
			else
			{
				tmpItem = tmpFactory.getItem(second);
			}
			tmpItem->setPosition(x * pixelSizeX, y * pixelSizeY);
			scene.items.push_back(tmpItem);
		}

		if (first == "Timeout")
		{
			int time;
			iss >> time;
			scene.gui->setTimeout(time);
		}

		if (first == "Timebuff")
		{
			int time;
			iss >> time;
			scene.gui->setTimebuffFactor(time);
		}

		if (first == "Text")
		{
			TextElement* element = new TextElement();
			std::string boldText = "";
			iss >> element->eventType;
			iss >> boldText;
			iss >> element->r;
			iss >> element->g;
			iss >> element->b;
			std::getline(infile, line);
			element->text = line;
			if (boldText=="bold")
			{
				element->bold = true;
			}
			scene.textBox->appendText(element);

		}

		if (first == "TriggerItem")
		{
			int x, y, x1, x2, y1, y2;
			iss >> x;
			iss >> y;
			iss >> x1;
			iss >> y1;
			iss >> x2;
			iss >> y2;

			TriggerItem *tmpItem = (TriggerItem*) tmpFactory.getItem("TriggerItem");
			tmpItem->setSwitchPos(x1, y1, x2, y2);
			tmpItem->setPosition(x * pixelSizeX, y * pixelSizeY);
			scene.items.push_back(tmpItem);
		}

	}
}
