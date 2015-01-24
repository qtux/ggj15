#include "SceneManager.hpp"
#include "Tile.hpp"
#include <SFML/Graphics.hpp>
#include "globals.hpp"

SceneManager::SceneManager(){
	colorToTilePositionMap[0x11941b] = sf::Vector2i(0,0); // grass
	colorToTilePositionMap[0x989898] = sf::Vector2i(0,3); // stone
	colorToTilePositionMap[0x9c6d27] = sf::Vector2i(0,1); // dirt
	colorToTilePositionMap[0x5f5f5f] = sf::Vector2i(0,2); // wet stone
	colorToTilePositionMap[0x000000] = sf::Vector2i(2,9); // wall

}

void SceneManager::showScene(std::string sceneName) {
	// load scene if not existent and set currentScene
}

void SceneManager::update(sf::Time deltaT) {
	scene.update(deltaT);
}

void SceneManager::loadScene(std::string name)
{
	Scene* tmpScene = new Scene();
	sf::Texture tmpTexture = textureManager.getTexture("./img/tilemap.png");
	sf::Texture leveltexture = textureManager.getTexture("./levels/colorCode.png");
	sf::Image levelImg = leveltexture.copyToImage();

	for (int x=0;x<Scene::sizeX;x++)
	{
		for (int y=0;y<Scene::sizeY;y++)
		{
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(tmpTexture);
			sf::Color tmpColor = levelImg.getPixel(x, y);
			sf::Uint32 colorKey = 0;
			colorKey &= tmpColor.r << 3*8;
			colorKey &= tmpColor.g << 1*8;
			colorKey &= tmpColor.b << 2*8;
			colorKey &= tmpColor.a << 0*8;

			sf::Vector2i tmpPos;

			std::map<sf::Uint32, sf::Vector2i>::const_iterator conIt = colorToTilePositionMap.find(colorKey);
			if (conIt != colorToTilePositionMap.end())
			{
				tmpPos = colorToTilePositionMap[colorKey];
			} else
			{
				tmpPos = sf::Vector2i(0, 0);
			}

			sprite->setTextureRect(sf::IntRect(tmpPos.x*Tile::pixelSizeX, tmpPos.y*Tile::pixelSizeY, Tile::pixelSizeX, Tile::pixelSizeY));
			Tile* tmpTile = new Tile();
			tmpTile->mySprite = sprite;
			tmpScene->setTile(tmpTile, x, y);
		}
	}



//	tmpTile->mySprite->setTexture()
}
