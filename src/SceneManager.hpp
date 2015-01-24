#pragma once

#include <vector>
#include <string>
#include "Scene.hpp"

class SceneManager {
private:
	// SceneLoader loader;
	std::vector<Scene> scene;
public:
	SceneManager();
	void showScene(std::string sceneName);
	void update(sf::Time deltaT);
	
	Scene currentScene;
};
