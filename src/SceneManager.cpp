#include "SceneManager.hpp"
#include "Tile.hpp"
#include "globals.hpp"
#include <iostream>

SceneManager::SceneManager(){
	colorToTilePositionMap[0x11941bff] = getRandomGrassPosition(); // grass
	colorToTilePositionMap[0x989898ff] = sf::Vector2i(0,3); // stone
	colorToTilePositionMap[0x9c6d27ff] = sf::Vector2i(0,1); // dirt
	colorToTilePositionMap[0x5f5f5fff] = sf::Vector2i(0,2); // wet stone
	colorToTilePositionMap[0x000000ff] = sf::Vector2i(2,9); // wall
	
	loadScene("levels/level000.png");
}


sf::Vector2i SceneManager::getRandomGrassPosition()
{
	return sf::Vector2i(0,0);
}

void SceneManager::showScene(std::string sceneName) {
	// load scene if not existent and set currentScene
}

void SceneManager::update(sf::Time deltaT) {
	scene.update(deltaT);
}

void SceneManager::loadScene(std::string file)
{
	// load textures for level
	tileTexture.loadFromFile("./img/TileMap.png");
	playerTexture.loadFromFile("./img/player.png");
	itemTexture.loadFromFile("./img/items.png");
	
	// load and set timebar
	timeBarTexture.loadFromFile("./img/timeBar.png");
	timeBarTexture.setRepeated(true);
	sf::Sprite *guiSprite = new sf::Sprite();
	guiSprite->setTexture(timeBarTexture);
	
	// load image bitmapt file
	sf::Image levelImg;
	levelImg.loadFromFile(file);
	
	// create sprites for each tile
	for (int x=0;x<Scene::sizeX * Scene::largeTileSizeX;x++)
	{
		for (int y=0;y<Scene::sizeY * Scene::largeTileSizeY;y++)
		{
			// set tile sprite texture
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(tileTexture);
			// get level code (from bitmap)
			sf::Color tmpColor = levelImg.getPixel(x, y);
			sf::Uint32 colorKey = 0;

			colorKey |= tmpColor.r << 3*8;
			colorKey |= tmpColor.g << 2*8;
			colorKey |= tmpColor.b << 1*8;
			colorKey |= tmpColor.a << 0*8;

			// resolve the correct tile based on the color code (and set correct texture rect)
			sf::Vector2i tmpPos;
			std::map<sf::Uint32, sf::Vector2i>::const_iterator conIt = colorToTilePositionMap.find(colorKey);
			if (conIt != colorToTilePositionMap.end())
			{
				tmpPos = colorToTilePositionMap[colorKey];
			} else
			{
				tmpPos = sf::Vector2i(0, 13);
			}

//			tmpPos = sf::Vector2i(0, 3);
			sprite->setTextureRect(sf::IntRect(tmpPos.x*Tile::pixelSizeX, tmpPos.y*Tile::pixelSizeY, Tile::pixelSizeX, Tile::pixelSizeY));
			// set position of the sprite inside the map
			sprite->setPosition(x*Tile::pixelSizeX, y*Tile::pixelSizeY);
			// create the tile and add it to the scene
			Tile* tmpTile = new Tile();
			tmpTile->mySprite = sprite;
			scene.setTile(tmpTile, x, y);
		}
	}
}
