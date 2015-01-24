#pragma once

#include <vector>
#include <string>
#include "Scene.hpp"
//#include "Game.hpp"
class Game;

class SceneManager {
public:
	SceneManager(Game* game);
	void showScene(std::string sceneName);
	void update(sf::Time deltaT);
	
	Scene currentScene;
private:
	// SceneLoader loader;
	std::vector<Scene> scene;
	Game* game;
};
