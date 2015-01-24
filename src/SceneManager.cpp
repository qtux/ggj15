#include "SceneManager.hpp"
#include "Tile.hpp"
#include <SFML/Graphics.hpp>
#include "globals.hpp"

//SceneManager::SceneManager(): currentScene(Scene()) {
//}

void SceneManager::showScene(std::string sceneName) {
	// load scene if not existent and set currentScene
}

void SceneManager::update(sf::Time deltaT) {
	scene.update(deltaT);
}

void SceneManager::loadScene(std::string name)
{
	Tile* tmpTile = new Tile();
	sf::Texture tmpTexture = textureManager.getTexture("./img/tilemap.png");

	for (int x=0;x<Scene::sizeX;x++)
	{
		for (int y=0;y<Scene::sizeY;y++)
		{
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(tmpTexture);
			sprite->setTextureRect(sf::IntRect(10, 10, 32, 32));
		}
	}



//	tmpTile->mySprite->setTexture()
}
