#include "SceneManager.hpp"
#include "Tile.hpp"
#include "globals.hpp"
#include <iostream>
#include "Player.hpp"
#include <string>
#include <sstream>
#include "TextFileParser.hpp"

SceneManager::SceneManager(){
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

	walkableTileState[0x000100ff] = false;
	walkableTileState[0x5f5f5fff] = true;
	walkableTileState[0x9b6d27ff] = true;
	walkableTileState[0x969896ff] = true;
	walkableTileState[0x11941bff] = true;
	
	currentLevelNumber = 3;
	nextLevel();
}

std::vector<sf::Vector2i>* SceneManager::checkNeighbours(sf::Uint32 color, int x, int y, sf::Image levelImg)
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

sf::Vector2i SceneManager::getTilePosition(sf::Uint32 color, int x, int y, sf::Image levelImg)
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

//sf::Vector2i SceneManager::getRandomGrassPosition()
//{
//	float rnd = rand() / RAND_MAX;
//	int grassCount = 4;
//	if (rnd < 1./grassCount)
//	{
//		return sf::Vector2i(0,0);
//	}
//	if (rnd < 2./grassCount)
//	{
//		return sf::Vector2i(1,0);
//	}
//	if (rnd < 3./grassCount)
//	{
//		return sf::Vector2i(2,0);
//	}
//
//	return sf::Vector2i(3,0);
//}

void SceneManager::showScene(std::string sceneName) {
	// load scene if not existent and set currentScene
}

void SceneManager::update(sf::Time deltaT) {
	//processEditMode();	// only for edit mode
	scene.update(deltaT);
}

sf::Uint32 SceneManager::createColorKey(sf::Color color) {
	
	sf::Uint32 colorKey = 0;
	
	colorKey |= color.r << 3*8;
	colorKey |= color.g << 2*8;
	colorKey |= color.b << 1*8;
	colorKey |= color.a << 0*8;
	
	return colorKey;
}

void SceneManager::loadScene(std::string fileName)
{
	scene = Scene();

	std::cout << fileName<< std::endl;
	tileTexture.loadFromFile(std::string(PATH) + "img/TileMap.png");
	playerTexture.loadFromFile(std::string(PATH) + "img/player.png");
	itemTexture.loadFromFile(std::string(PATH) + "img/items.png");
	// load and set timebar
	GUI* gui = new GUI();
	gui->setTimeout(20);
	scene.setGUI(gui);
	// load image bitmapt file
	sf::Image levelImg;
	levelImg.loadFromFile(fileName+".png");
	
	// create sprites for each tile
	for (int x = 0; x < sizeX * largeTileSizeX; ++x)
	{
		for (int y = 0; y < sizeY * largeTileSizeY; ++y)
		{
			// set tile sprite texture
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(textureManager.tileTexture);
			// get level code (from bitmap)
			sf::Color tmpColor = levelImg.getPixel(x, y);
			
			// resolve the correct tile based on the color code (and set correct texture rect)
			sf::Vector2i tmpPos;
			sf::Uint32 colorKey = createColorKey(tmpColor);
			tmpPos = getTilePosition(colorKey, x, y, levelImg);

//			tmpPos = sf::Vector2i(0, 3);
			sprite->setTextureRect(sf::IntRect(tmpPos.x * pixelSizeX, tmpPos.y * pixelSizeY, pixelSizeX, pixelSizeY));
			// set position of the sprite inside the map
			sprite->setPosition(x * pixelSizeX, y * pixelSizeY);
			// create the tile and add it to the scene
			Tile* tmpTile = new Tile();
			tmpTile->walkable = walkableTileState[colorKey];
			tmpTile->mySprite = sprite;
			scene.setTile(tmpTile, x, y);
		}
	}
	scene.player = new Player();
	sf::Sprite *playerSprite = new sf::Sprite();
	sf::Sprite *doggieSprite = new sf::Sprite();
	playerSprite->setTexture(textureManager.playerTexture);
	playerSprite->setPosition(90,90);
	doggieSprite->setTexture(textureManager.playerTexture);
	doggieSprite->setPosition(90,90);
	scene.player->mySprite = playerSprite;

	scene.player->doggieSprite = doggieSprite;

	// read text file
	TextFileParser::loadTextFile(scene, fileName + ".txt");
	scene.textBox->triggerText("start");
}

void SceneManager::processEditMode()
{
	for (int key=4;key<13;key++)
	{
		if (input[key] == true)
		{
			sf::Vector2i globalPosition = sf::Mouse::getPosition(window);

			sf::Vector2f localPosition;
			localPosition.x = 1.f * globalPosition.x / (pixelSizeX);
			localPosition.y = 1.f * globalPosition.y / (pixelSizeY);
//			std::cout<<localPosition.x<<", "<<localPosition.y<<std::endl;


			sf::Vector2i tmpPos;
			tmpPos.x = (int)(localPosition.x);
			tmpPos.y = (int)(localPosition.y);

			GameObject* tmpTile = scene.getTile(tmpPos.x, tmpPos.y);
			Tile* newTile = new Tile();

			newTile->mySprite = tmpTile->mySprite;

			sf::Vector2i posInTexture = sf::Vector2i(0, 0); // grass
			newTile->mySprite->setTextureRect(sf::IntRect(posInTexture.x * pixelSizeX, posInTexture.y * pixelSizeY, pixelSizeX, pixelSizeY));
			scene.setTile(newTile,tmpPos.x, tmpPos.y);
		}
	}
}

void SceneManager::nextLevel(){
	currentLevelNumber++;
	loadScene(std::string(PATH) + "levels/level"+std::to_string(currentLevelNumber));
}

void SceneManager::restartLevel(){
	loadScene(std::string(PATH) + "levels/level"+std::to_string(currentLevelNumber));
}

Scene& SceneManager::getCurrentScene()
{
	return scene;
}

int SceneManager::getCurrentLevelNumber()
{
	return currentLevelNumber;
}
