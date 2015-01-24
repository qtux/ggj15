#pragma once

#include <vector>
#include <string>
#include <map>
#include "Scene.hpp"

class SceneManager {
private:
	// SceneLoader loader;
	Scene scene;
	std::map<sf::Uint32, sf::Vector2i> colorToTilePositionMap;

	sf::Vector2i getRandomGrassPosition();

public:
	SceneManager();
	void showScene(std::string sceneName);
	void update(sf::Time deltaT);
	void loadScene(std::string name);
	
//	Scene currentScene;
};
