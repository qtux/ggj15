#pragma once

#include <vector>
#include <string>
#include "Scene.hpp"

class SceneManager {
private:
	// SceneLoader loader;
	std::vector<Scene> scene;
	Game* game;
public:
	SceneManager(Game* game);
	void showScene(std::string sceneName);
	void update(sf::Time deltaT);
	
	Scene currentScene;
};
