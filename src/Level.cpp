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
#include "Items/KeyItem.hpp"
#include "TextFileParser.hpp"


Level::Level():
	Scene(gb::screenWidth, gb::screenHeight)
{
	gameBoard.resize(gb::sizeX * gb::sizeY * gb::largeTileSizeX * gb::largeTileSizeY);
	textBox = new TextBox();
	leaved = false;
	highscore  = 0;
	fooexit = false;
	
	// moved
	std::vector<sf::Vector2i> *tmpVector = new std::vector<sf::Vector2i>();
	tmpVector->push_back(sf::Vector2i(0,0));
	tmpVector->push_back(sf::Vector2i(1,0));
	tmpVector->push_back(sf::Vector2i(2,0));
	tmpVector->push_back(sf::Vector2i(3,0));
	tmpVector->push_back(sf::Vector2i(4,0));
	colorToTilePositionMap[0x11941bff] = tmpVector; // grass
	tmpVector = new std::vector<sf::Vector2i>();
	tmpVector->push_back(sf::Vector2i(0,3));
	tmpVector->push_back(sf::Vector2i(1,3));
	tmpVector->push_back(sf::Vector2i(2,3));
	tmpVector->push_back(sf::Vector2i(3,3));
	tmpVector->push_back(sf::Vector2i(4,3));
	tmpVector->push_back(sf::Vector2i(5,3));
	tmpVector->push_back(sf::Vector2i(6,3));
	tmpVector->push_back(sf::Vector2i(7,3));
	tmpVector->push_back(sf::Vector2i(8,3));
	tmpVector->push_back(sf::Vector2i(9,3));
	colorToTilePositionMap[0x969896ff] = tmpVector; // stone
	tmpVector = new std::vector<sf::Vector2i>();
	tmpVector->push_back(sf::Vector2i(0,1));
	tmpVector->push_back(sf::Vector2i(1,1));
	tmpVector->push_back(sf::Vector2i(2,1));
	tmpVector->push_back(sf::Vector2i(3,1));
	tmpVector->push_back(sf::Vector2i(4,1));
	colorToTilePositionMap[0x9b6d27ff] = tmpVector; // dirt
	tmpVector = new std::vector<sf::Vector2i>();
	tmpVector->push_back(sf::Vector2i(0,2));
	tmpVector->push_back(sf::Vector2i(1,2));
	tmpVector->push_back(sf::Vector2i(2,2));
	tmpVector->push_back(sf::Vector2i(3,2));
	tmpVector->push_back(sf::Vector2i(4,2));
	colorToTilePositionMap[0x5f5f5fff] = tmpVector; // wet stone
	tmpVector = new std::vector<sf::Vector2i>();
	tmpVector->push_back(sf::Vector2i(2,9));
	colorToTilePositionMap[0x000100ff] = tmpVector; // wall
	tmpVector = new std::vector<sf::Vector2i>();
	tmpVector->push_back(sf::Vector2i(5,2));
	tmpVector->push_back(sf::Vector2i(6,2));
	tmpVector->push_back(sf::Vector2i(7,2));
	tmpVector->push_back(sf::Vector2i(8,2));
	tmpVector->push_back(sf::Vector2i(9,2));
	colorToTilePositionMap[0x0000abff] = tmpVector; // water
	tmpVector = new std::vector<sf::Vector2i>();
	tmpVector->push_back(sf::Vector2i(10,0));
	tmpVector->push_back(sf::Vector2i(11,0));
	tmpVector->push_back(sf::Vector2i(12,0));
	tmpVector->push_back(sf::Vector2i(13,0));
	tmpVector->push_back(sf::Vector2i(14,0));
	colorToTilePositionMap[0x003E04ff] = tmpVector; // trees

	walkableTileState[0x000100ff] = false;
	walkableTileState[0x5f5f5fff] = true;
	walkableTileState[0x9b6d27ff] = true;
	walkableTileState[0x969896ff] = true;
	walkableTileState[0x11941bff] = true;
	walkableTileState[0x003E04ff] = false;
}

GameObject* Level::getTile(int x, int y)
{
	if (x + y*gb::sizeX < (int)gameBoard.size())
	{
		return gameBoard[x + y * gb::sizeX * gb::largeTileSizeX];
	}
	return 0;
}


void Level::setTile(GameObject* obj, int x, int y)
{
	gameBoard[x + y * gb::sizeX * gb::largeTileSizeX] = obj;
}

void Level::setGUI(GUI* obj)
{
	gui = obj;
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
			tmp.scale = 1.f;
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
			tmp.scale = 1.f;
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
	float scaleMax = 1.4;
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
				tmpObj.tile->mySprite->setScale(1, 1);
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
//	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
//	{
//		sf::Vector2i globalPosition = sf::Mouse::getPosition(gb::window);
//
//		sf::Vector2f localPosition;
//		localPosition.x = 1.f*globalPosition.x/(gb::pixelSizeX);
//		localPosition.y = 1.f*globalPosition.y/(gb::pixelSizeY);
//		std::cout<<localPosition.x<<", "<<localPosition.y<<std::endl;
//	}
	/*for (std::vector<GameObject*>::iterator it = gameBoard.begin();it != gameBoard.end(); it++)
	{
		(*it)->update(deltaT);
		std::cout << (*it) << std::endl;
	}*/
	if (highscore != 0)
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

	sf::Font font;
	font.loadFromFile(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf");

	sf::Text level;
	level.setFont(font);
	level.setPosition(gb::gridWidth + 2, gb::gridHeight - 32);
	level.setString(std::to_string(gb::sceneManager.getCurrentLevelNumber()+1));
	gb::window.draw(level);
	textBox->update(deltaT);
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

void Level::draw(sf::RenderTarget &renderTarget)
{
	for(auto& obj: gameBoard) {
		obj->draw(renderTarget, &shader);
	}

	for(auto& obj: items) {
		obj->draw(renderTarget, &shader);
	}
	player->draw(renderTarget, &shader);
}

void Level::finishLevel()
{
	highscore = new Highscore(gb::sceneManager.getCurrentLevelNumber()+1);
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
	return this;
}

// moved
std::vector<sf::Vector2i>* Level::checkNeighbours(sf::Uint32 color, int x, int y, sf::Image levelImg)
{
	std::vector<sf::Vector2i> *tmpVector = new std::vector<sf::Vector2i>();
	int h = 24;
	int w = 30;
	if (color == 0x11941bff) // grass
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (x > 0 && y > 0 && x < w - 1 && y < h - 1) {
					sf::Color tmpColor = levelImg.getPixel((x-1) + i, (y-1) + j);
					sf::Uint32 colorKey = createColorKey(tmpColor);
					// if a non-green border piece is found: choose a mix-tile
					if (colorKey != 0x11941bff)
					{
						tmpVector->push_back(sf::Vector2i(5,0));
						tmpVector->push_back(sf::Vector2i(6,0));
						tmpVector->push_back(sf::Vector2i(7,0));
						tmpVector->push_back(sf::Vector2i(8,0));
						tmpVector->push_back(sf::Vector2i(9,0));
						tmpVector->push_back(sf::Vector2i(4,5));
						goto stopGreen;
					}
				}
			}
		}
		stopGreen: ;
	}
	// this one my be omitted because it does not have much of an effect
	if (color == 0x969896ff) // stone
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (x > 0 && y > 0 && x < w - 1 && y < h - 1) {
					sf::Color tmpColor = levelImg.getPixel((x-1) + i, (y-1) + j);
					sf::Uint32 colorKey = createColorKey(tmpColor);
					// if a green or brown border next to piece is found: choose a border tile
					if (colorKey == 0x11941bff)
					{
						tmpVector->push_back(sf::Vector2i(3 + i, 4 + j));
					}
					if (colorKey == 0x9b6d27ff)
					{
						tmpVector->push_back(sf::Vector2i(6 + i, 4 + j));
					}
				}
			}
		}
	}
	if (color == 0x9b6d27ff) // dirt
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (x > 0 && y > 0 && x < w - 1 && y < h - 1) {
					sf::Color tmpColor = levelImg.getPixel((x-1) + i, (y-1) + j);
					sf::Uint32 colorKey = createColorKey(tmpColor);
					// if a non-brown border piece is found: choose a mix-tile
					if (colorKey != 0x9b6d27ff)
					{
						tmpVector->push_back(sf::Vector2i(5,1));
						tmpVector->push_back(sf::Vector2i(6,1));
						tmpVector->push_back(sf::Vector2i(7,1));
						tmpVector->push_back(sf::Vector2i(8,1));
						tmpVector->push_back(sf::Vector2i(9,1));
						tmpVector->push_back(sf::Vector2i(7,5));
						goto stopBrown;
					}
				}
			}
		}
		stopBrown: ;
	}
	if (color == 0x5f5f5fff) // wet stone
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (x > 0 && y > 0 && x < w - 1 && y < h - 1) {
					sf::Color tmpColor = levelImg.getPixel((x-1) + i, (y-1) + j);
					sf::Uint32 colorKey = createColorKey(tmpColor);
					// if a dark grey border near piece is found: choose a border tile
					if (colorKey == 0x969896ff)
					{
						tmpVector->push_back(sf::Vector2i(i, 4 + j));
					}
				}
			}
		}
	}
	if (color == 0x000100ff) // wall
	{
		// TODO tempVector Zuweisungen auslagern
		
		// gameboard borders
		if (x == 0)
		{
			sf::Color tmpColor = levelImg.getPixel(x + 1, y);
			sf::Uint32 colorKey = createColorKey(tmpColor);
			if (colorKey != 0x000100ff)
			{
				tmpVector->push_back(sf::Vector2i(0,9));
				tmpVector->push_back(sf::Vector2i(0,10));
				tmpVector->push_back(sf::Vector2i(0,11));
				goto doneSorting;
			}
		}
		
		if (x == w - 1)
		{
			sf::Color tmpColor = levelImg.getPixel(x - 1, y);
			sf::Uint32 colorKey = createColorKey(tmpColor);
			if (colorKey != 0x000100ff)
			{
				tmpVector->push_back(sf::Vector2i(0,9));
				tmpVector->push_back(sf::Vector2i(0,10));
				tmpVector->push_back(sf::Vector2i(0,11));
				goto doneSorting;
			}
		}
		if (y == 0)
		{
			sf::Color tmpColor = levelImg.getPixel(x, y + 1);
			sf::Uint32 colorKey = createColorKey(tmpColor);
			if (colorKey != 0x000100ff)
			{
				tmpVector->push_back(sf::Vector2i(1,7));
				tmpVector->push_back(sf::Vector2i(2,7));
				tmpVector->push_back(sf::Vector2i(3,7));
				goto doneSorting;
			}
		}
		
		if (y == h - 1)
		{
			sf::Color tmpColor = levelImg.getPixel(x, y - 1);
			sf::Uint32 colorKey = createColorKey(tmpColor);
			if (colorKey != 0x000100ff)
			{
				tmpVector->push_back(sf::Vector2i(1,7));
				tmpVector->push_back(sf::Vector2i(2,7));
				tmpVector->push_back(sf::Vector2i(3,7));
				goto doneSorting;
			}
		}
		
		
		// gameboard mid slender walls
		if (x > 0 && x < w - 1)
		{
			sf::Color tmpColor1 = levelImg.getPixel(x - 1, y);
			sf::Uint32 colorKey1 = createColorKey(tmpColor1);
			sf::Color tmpColor2 = levelImg.getPixel(x + 1, y);
			sf::Uint32 colorKey2 = createColorKey(tmpColor2);
		
			// if no wall left and right
			if (colorKey1 != 0x000100ff && colorKey2 != 0x000100ff)
			{
				tmpVector->push_back(sf::Vector2i(0,9));
				tmpVector->push_back(sf::Vector2i(0,10));
				tmpVector->push_back(sf::Vector2i(0,11));
				goto doneSorting;
			}
		}
		
		if (y > 0 && y < h - 1)
		{
			sf::Color tmpColor1 = levelImg.getPixel(x, y - 1);
			sf::Uint32 colorKey1 = createColorKey(tmpColor1);
			sf::Color tmpColor2 = levelImg.getPixel(x, y + 1);
			sf::Uint32 colorKey2 = createColorKey(tmpColor2);
		
			// if no wall top and down
			if (colorKey1 != 0x000100ff && colorKey2 != 0x000100ff)
			{
				tmpVector->push_back(sf::Vector2i(1,7));
				tmpVector->push_back(sf::Vector2i(2,7));
				tmpVector->push_back(sf::Vector2i(3,7));
				goto doneSorting;
			}
		}
		
		/*if (x > 0 && x < w - 1 && y > 0 && y < h - 1)
		  {
			sf::Color tmpColor1 = levelImg.getPixel(x, y - 1);
			sf::Uint32 colorKey1 = createColorKey(tmpColor1);
			sf::Color tmpColor2 = levelImg.getPixel(x, y + 1);
			sf::Uint32 colorKey2 = createColorKey(tmpColor2);
			sf::Color tmpColor3 = levelImg.getPixel(x + 1, y);
			sf::Uint32 colorKey3 = createColorKey(tmpColor3);
			sf::Color tmpColor4 = levelImg.getPixel(x - 1, y);
			sf::Uint32 colorKey4 = createColorKey(tmpColor4);
			
			// if no wall top and down
			if (colorKey1 == 0x000100ff && colorKey2 == 0x000100ff && colorKey3 == 0x000100ff && colorKey4 == 0x000100ff)
			{
				tmpVector->push_back(sf::Vector2i(2,11));
			}
		}*/
				
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (x > 0 && y > 0 && x < w - 1 && y < h - 1) {
					sf::Color tmpColor = levelImg.getPixel((x-1) + i, (y-1) + j);
					sf::Uint32 colorKey = createColorKey(tmpColor);
					// check wall mass nearby
					if (colorKey != 0x000100ff)
					{
						tmpVector->push_back(sf::Vector2i(1 + i, 8 + j));
						goto doneSorting;
					}
				}
			}
		}
		// if all is a wall
		tmpVector->push_back(sf::Vector2i(2, 9));
	}
	
	doneSorting:
	
	return tmpVector;
}

sf::Vector2i Level::getTilePosition(sf::Uint32 color, int x, int y, sf::Image levelImg)
{
	std::map<sf::Uint32, std::vector<sf::Vector2i>*>::const_iterator conIt = colorToTilePositionMap.find(color);
	if (conIt != colorToTilePositionMap.end())
	{
		
		std::vector<sf::Vector2i> *tmpVec = checkNeighbours(color, x, y, levelImg);
		
		if (tmpVec->empty())
		{
			tmpVec = colorToTilePositionMap[color];
		}

		int numTiles = tmpVec->size();
		float rnd = 1.f * rand() / RAND_MAX;

		for (int i=0;i<numTiles;i++)
		{
			if (rnd < 1.f*i/numTiles)
			{
				return (*tmpVec)[i];
			}
		}

		return (*tmpVec)[numTiles-1];
	}
	return sf::Vector2i(0, 13);
}

sf::Uint32 Level::createColorKey(sf::Color color) {
	
	sf::Uint32 colorKey = 0;
	
	colorKey |= color.r << 3*8;
	colorKey |= color.g << 2*8;
	colorKey |= color.b << 1*8;
	colorKey |= color.a << 0*8;
	
	return colorKey;
}

void Level::loadScene(std::string fileName)
{
	gb::showOutline = true;
	// load and set timebar
	GUI* gui = new GUI();
	gui->setTimeout(20);
	setGUI(gui);
	// load image bitmapt file
	sf::Image levelImg;
	
	// try to load the file
	if (!levelImg.loadFromFile(fileName+".png")) {
		gb::inMenu = true;
		return;
	}
	
	// create sprites for each tile
	for (int x = 0; x < gb::sizeX * gb::largeTileSizeX; ++x)
	{
		for (int y = 0; y < gb::sizeY * gb::largeTileSizeY; ++y)
		{
			// set tile sprite texture
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/TileMap.png", false));
			// get level code (from bitmap)
			sf::Color tmpColor = levelImg.getPixel(x, y);
			
			// resolve the correct tile based on the color code (and set correct texture rect)
			sf::Vector2i tmpPos;
			sf::Uint32 colorKey = createColorKey(tmpColor);
			tmpPos = getTilePosition(colorKey, x, y, levelImg);

//			tmpPos = sf::Vector2i(0, 3);
			sprite->setTextureRect(sf::IntRect(tmpPos.x * gb::pixelSizeX, tmpPos.y * gb::pixelSizeY, gb::pixelSizeX, gb::pixelSizeY));
			// set position of the sprite inside the map
			sprite->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			// create the tile and add it to the scene
			Tile* tmpTile = new Tile();
			tmpTile->walkable = walkableTileState[colorKey];
			tmpTile->mySprite = sprite;
			setTile(tmpTile, x, y);
		}
	}
	player = new Player();
	sf::Sprite *playerSprite = new sf::Sprite();
	sf::Sprite *doggieSprite = new sf::Sprite();
	playerSprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/player.png", false));
	playerSprite->setPosition(90,90);
	doggieSprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/player.png", false));
	doggieSprite->setPosition(90,90);
	player->mySprite = playerSprite;

	player->doggieSprite = doggieSprite;

	// read text file
	TextFileParser::loadTextFile(this, fileName + ".txt");
	textBox->triggerText("start");
}
