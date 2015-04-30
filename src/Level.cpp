/*
 * Level.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#include "Level.hpp"
#include "Tile.hpp"
#include "global.hpp"
#include "GUI.hpp"
#include <math.h>
#include "Player.hpp"
#include "Menu.hpp"
#include "Items/KeyItem.hpp"

// parser
#include <fstream>
#include <sstream>
#include "ItemFactory.hpp"
#include "Items/TriggerItem.hpp"	// --> move to ItemFactory?

// shader
#include <iostream>

Level::Level(unsigned int number):
	Scene({gb::screenWidth, gb::screenHeight}),
	number(number)
{
	reset();
	// load only the fragment shader
	_fragmentShader.loadFromFile("src/shader/fragment_shader.frag", sf::Shader::Fragment);
	if (!_fragmentShader.isAvailable())
	{
	    std::cerr<<"could not load shader"<<std::endl;
	}
}

void Level::reset()
{
	//background.setTexture(&gb::textureManager.getTexture("img/background.png", true));
	// set background and outline
	outline.setOutlineColor(sf::Color(0x90, 0x90, 0x00));
	outline.setFillColor(sf::Color(0x00, 0x00, 0x00, 0x00));
	outline.setOutlineThickness(2.0f);
	outline.setPosition(0, 0);
	outline.setSize(sf::Vector2f(gb::sizeX * gb::largeTileSizeX * gb::pixelSizeX, gb::sizeY * gb::largeTileSizeY * gb::pixelSizeY));
	
	// TODO implement reset of the level (new init)
	gameBoard.resize(gb::sizeX * gb::sizeY * gb::largeTileSizeX * gb::largeTileSizeY);
	textBox = new TextBox();
	leaved = false;
	highscore  = nullptr;
	fooexit = false;
	
	// load and set timebar
	gui = new GUI(this);
	gui->setTimeout(20);
	// load image bitmapt file
	sf::Image levelImg;
	
	// try to load the file
	std::string fileName = std::string(PATH) + "levels/level" + std::to_string(number);
	if (!levelImg.loadFromFile(fileName+".png")) {
		gb::inMenu = true;
		return;
	}
	
	// create a tileset
	std::map<sf::Uint32, bool> walkableTileState;
	walkableTileState[0x000100ff] = false;	// wall
	walkableTileState[0x5f5f5fff] = true;	// wet stone
	walkableTileState[0x9b6d27ff] = true;	// dirt
	walkableTileState[0x969896ff] = true;	// stone
	walkableTileState[0x11941bff] = true;	// grass
	walkableTileState[0x003E04ff] = false;	// trees
	walkableTileState[0x0000abff] = false;	// water
	
	std::map<sf::Uint32, unsigned int> colorToInt;
	colorToInt[0x000100ff] = 6;	// wall
	colorToInt[0x5f5f5fff] = 4;	// wet stone
	colorToInt[0x9b6d27ff] = 3;	// dirt
	colorToInt[0x969896ff] = 2;	// stone
	colorToInt[0x11941bff] = 1;	// grass
	colorToInt[0x003E04ff] = 5;	// trees
	colorToInt[0x0000abff] = 0;	// water
	
	const sf::Vector2u gridSize(gb::sizeX * gb::largeTileSizeX, gb::sizeY * gb::largeTileSizeY);
	// TODO determine the following two variables in another way
	const sf::Vector2u tileSize(32, 32);
	const sf::Vector2f scale(0.5f, 0.5f);
	
	std::vector<unsigned int> mapping;
	for (int x = 0; x < gridSize.y; ++x)
	{
		for (int y = 0; y < gridSize.x; ++y)
		{
			sf::Uint32 colorKey = createColorKey(levelImg.getPixel(y, x));
			mapping.push_back(colorToInt[colorKey]);
		}
	}
	
	const sf::Texture& baseTileSet = gb::textureManager.getTexture(std::string(PATH) + "img/tileset.png", false);
	sf::Vector2f offset(-6, -6);
	const sf::Texture& tileSet = gb::textureManager.getTileSet(baseTileSet, mapping, tileSize, gridSize, offset);
	
	// create sprites for each tile
	for (int x = 0; x < gridSize.x; ++x)
	{
		for (int y = 0; y < gridSize.y; ++y)
		{
			// create tile sprite
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(tileSet);
			sprite->setTextureRect(sf::IntRect(x * tileSize.x, y * tileSize.y, tileSize.x, tileSize.y));
			sprite->setScale(scale);
			sprite->setPosition(x * tileSize.x * scale.x, y * tileSize.y * scale.y);
			
			// create the tile and add it to the scene
			Tile* tmpTile = new Tile();
			sf::Uint32 colorKey = createColorKey(levelImg.getPixel(x, y));
			tmpTile->walkable = walkableTileState[colorKey];
			tmpTile->mySprite = sprite;
			gameBoard[x + y * gridSize.x] = tmpTile;
		}
	}
	
	player = new Player(this);
	sf::Sprite *playerSprite = new sf::Sprite();
	sf::Sprite *doggieSprite = new sf::Sprite();
	playerSprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/player.png", false));
	playerSprite->setPosition(90,90);
	doggieSprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/player.png", false));
	doggieSprite->setPosition(90,90);
	player->mySprite = playerSprite;
	player->doggieSprite = doggieSprite;
	
	// read text file
	std::ifstream infile(fileName + ".txt");
	std::string line;
	
	sf::Sprite *itemSprite = new sf::Sprite();
	itemSprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/items.png", false));
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
			startPos.x = x * gb::pixelSizeX;
			startPos.y = y * gb::pixelSizeY;
			player->setPosition(startPos.x, startPos.y);
			player->doggieSprite->setPosition(startPos.x, startPos.y);
		}
		if (first == "Portal")
		{
			int x,y;
			iss >> x;
			iss >> y;
			portalPos.x = x * gb::pixelSizeX;
			portalPos.y = y * gb::pixelSizeY;
			Item *tmpItem = tmpFactory.getItem("PortalItem");
			tmpItem->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			items.push_back(tmpItem);
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
			else if (second == "DoorItem")
			{
				bool vertical = false;
				bool closed = true;
				iss >> vertical >> closed;
				tmpItem = tmpFactory.getItem(second, closed, -1, -1, -1, -1, vertical);
			}
			else
			{
				tmpItem = tmpFactory.getItem(second);
			}
			tmpItem->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			items.push_back(tmpItem);
		}
		
		if (first == "Timeout")
		{
			int time;
			iss >> time;
			gui->setTimeout(time);
		}
		if (first == "Timebuff")
		{
			int time;
			iss >> time;
			gui->setTimebuffFactor(time);
		}
		if (first == "Text")
		{
			TextBox::TextElement* element = new TextBox::TextElement();
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
			textBox->appendText(element);
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
			tmpItem->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			items.push_back(tmpItem);
		}
		if (first == "TriggerTrapItem")
		{
			int x, y, x1, x2, y1, y2;
			iss >> x;
			iss >> y;
			iss >> x1;
			iss >> y1;
			iss >> x2;
			iss >> y2;
			TriggerItem *tmpItem = (TriggerItem*) tmpFactory.getItem("TriggerTrapItem");
			tmpItem->setSwitchPos(x1, y1, x2, y2);
			tmpItem->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			items.push_back(tmpItem);
		}
	}
	
	// trigger text
	textBox->triggerText("start");
}

GameObject* Level::getTile(int x, int y)
{
	if (x + y*gb::sizeX < (int)gameBoard.size())
	{
		return gameBoard[x + y * gb::sizeX * gb::largeTileSizeX];
	}
	return 0;
}

const std::vector<GameObject*> &Level::getGameBoard() const
{
	return gameBoard;
}

void Level::switchLargeTile(int x1, int y1, int x2, int y2)
{
	int startX1 = x1*gb::largeTileSizeX;
	int startY1 = y1*gb::largeTileSizeY;
	int startX2 = x2*gb::largeTileSizeX;
	int startY2 = y2*gb::largeTileSizeY;

	sf::Vector2f orthogonal, dir;
	float length;
	float momMax = 80.f;

	for (int x=0;x<gb::largeTileSizeX;x++)
	{
		for (int y=0;y<gb::largeTileSizeY;y++)
		{
			sf::Vector2f tmpPos = getTile(startX1+x, startY1+y)->getPosition();
			sf::Vector2f tmpPos2 = getTile(startX2+x, startY2+y)->getPosition();
//			getTile(startX2+x, startY2+y)->setPosition(tmpPos.x, tmpPos.y);
//			getTile(startX1+x, startY1+y)->setPosition(tmpPos2.x, tmpPos2.y);

			TileFlightData tmp;
			tmp.startPos = tmpPos;
			tmp.currentPos = tmpPos;
			tmp.targetPos = tmpPos2;
			tmp.tile = getTile(startX1+x, startY1+y);
			dir = tmp.targetPos - tmp.currentPos;
			dir *= 0.2f;
			orthogonal.x = -dir.y;
			orthogonal.y = dir.x;
			length = sqrt(orthogonal.x*orthogonal.x+orthogonal.y*orthogonal.y);
			orthogonal = orthogonal * 1.f/length;
			tmp.momentum.x = momMax * (2.f * (1.f*rand() / RAND_MAX) - 1.f);
			tmp.momentum.y = momMax * (2.f * (1.f*rand() / RAND_MAX) - 1.f);
			tmp.scale = 0.5f;
			tileAnimationPos.push_back(tmp);

			tmp.startPos = tmpPos2;
			tmp.currentPos = tmpPos2;
			tmp.targetPos = tmpPos;
			tmp.tile = getTile(startX2+x, startY2+y);
			dir = tmp.targetPos -tmp.currentPos;
			length = sqrt(dir.x*dir.x+dir.y*dir.y);
			dir = dir * 1.f/length;
			orthogonal.x = -dir.y;
			orthogonal.y = dir.x;
//			length = sqrt(orthogonal.x*orthogonal.x+orthogonal.y*orthogonal.y);
//			orthogonal = orthogonal * 1.f/length;
			tmp.momentum.x = momMax * (2.f * (1.f*rand() / RAND_MAX) - 1.f);
			tmp.momentum.y = momMax * (2.f * (1.f*rand() / RAND_MAX) - 1.f);
			tmp.scale = 0.5f;
//			tmp.momentum = orthogonal * 40.f * (2.f * (1.f*rand() / RAND_MAX) - 1.f);
			tileAnimationPos.push_back(tmp);
		}
	}
	tileAnimationTime = 1.5f;

}

void Level::updateTileAnimation(sf::Time deltaT)
{
	float dt = deltaT.asSeconds() * 1000;
	tileAnimationTime -= dt / 1000;
	float scaleMax = 0.7f;
		for(std::vector<TileFlightData>::iterator itIt = tileAnimationPos.begin() ; itIt != tileAnimationPos.end() ; ) {
			TileFlightData &tmpObj = (*itIt);
			tmpObj.momentum = tmpObj.momentum * 0.95f + (tmpObj.targetPos - tmpObj.currentPos)* 0.02f;
			sf::Vector2f dir = tmpObj.momentum;
			tmpObj.currentPos += dt*dir * 0.01f;
			sf::Vector2f distVec1 = tmpObj.currentPos- tmpObj.startPos;
			sf::Vector2f distVec2 = tmpObj.currentPos- tmpObj.targetPos;
			sf::Vector2f distTotalVec = tmpObj.startPos- tmpObj.targetPos;
			float dist1 = sqrt(distVec1.x*distVec1.x+distVec1.y*distVec1.y);
			float dist2 = sqrt(distVec2.x*distVec2.x+distVec2.y*distVec2.y);
			float distTotal = sqrt(distTotalVec.x*distTotalVec.x+distTotalVec.y*distTotalVec.y);
			tmpObj.scale = (std::min(dist1,dist2)+distTotal) / distTotal;
			tmpObj.tile->mySprite->setScale(scaleMax*tmpObj.scale, scaleMax*tmpObj.scale);
			tmpObj.tile->setPosition(tmpObj.currentPos.x, tmpObj.currentPos.y);

			// delete animation if target is reached
			if (dir.x*dir.x+dir.y*dir.y < 10 || tileAnimationTime < 0)
			{
				tmpObj.tile->setPosition(tmpObj.targetPos.x, tmpObj.targetPos.y);
				tmpObj.tile->mySprite->setScale(0.5f, 0.5f);
				itIt = tileAnimationPos.erase(itIt);
			}
			else
			{
				itIt ++;
			}
		}
}

void Level::update(sf::Time deltaT, sf::RenderWindow& window)
{
	if (highscore != nullptr)
	{
		highscore->update(deltaT);
		return;
	}
	if (leaved && !textBox->enabled())
	{
		finishLevel();
	}

	updateTileAnimation(deltaT);

	for(auto& obj: gameBoard) {
		obj->update(deltaT);
	}

	for(auto& obj: items) {
		obj->update(deltaT);
	}
	player->update(deltaT);
	if (gui != 0)
	{
		gui->update(deltaT);
	}
	textBox->update();
	textBox->draw(window);
	if (!fooexit){
		for(std::vector<Item*>::iterator itIt = items.begin() ; itIt != items.end() ; ) {
			if (player->intersects(**itIt))
			{
				if ((*itIt)->applyEffect())
				{
					fooexit = true;
					return;
				}
				if ((*itIt)->collectable)
				{
					itIt = items.erase(itIt);
				}
				else
				{
					itIt ++;
				}
			}
			else
			{
				itIt ++;
			}
		}
	}
}

void Level::draw(sf::RenderTarget &renderTarget, bool focus)
{
	//renderTarget.draw(background);	// TODO reenable me
	renderTarget.draw(outline);
	for(auto& obj: gameBoard) {
		obj->draw(renderTarget, nullptr);
	}

	for(auto& obj: items) {
		obj->draw(renderTarget, nullptr);
	}
	player->draw(renderTarget, nullptr);
	gui->draw(renderTarget);
}

void Level::finishLevel()
{
	highscore = new Highscore(this);
	highscore->save();
	highscore->load();
}


bool Level::readyToLeave() const
{
	int size = items.size();
	int keysInLevel = 0;
	for(int i = 0;i < size;i++)
	{
		if (dynamic_cast<KeyItem*>(items[i]))
		{
			keysInLevel++;
		}
	}
	return (keysInLevel == 0);
}


void Level::leave()
{
	if (!readyToLeave()) return;
	leaved = true;
	textBox->triggerText("end");
	if (!textBox->enabled())
	{
		finishLevel();
	}
}

Scene* Level::processEvent(sf::Event event, sf::RenderWindow& window)
{
	// preprocessing key input (to enhance code readability)
	sf::Keyboard::Key keyPressed = sf::Keyboard::Unknown;
	if (event.type == sf::Event::KeyPressed)
	{
		keyPressed = event.key.code;
	}
	
	// process events
	if (keyPressed == sf::Keyboard::Escape)
	{
		return new Menu(Menu::Command::LEVEL);
	}
	return this;
}
sf::Uint32 Level::createColorKey(sf::Color color) {
	
	sf::Uint32 colorKey = 0;
	
	colorKey |= color.r << 3*8;
	colorKey |= color.g << 2*8;
	colorKey |= color.b << 1*8;
	colorKey |= color.a << 0*8;
	
	return colorKey;
}
