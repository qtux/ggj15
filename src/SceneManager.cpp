#include "SceneManager.hpp"
#include "Game.hpp"

SceneManager::SceneManager(Game* game): game(game) {
	// load default scene and show it
	showScene("default");
}

void SceneManager::showScene(std::string sceneName) {
	// load scene if not existent
	currentScene = Scene();
}

void SceneManager::update(sf::Time deltaT) {
	
}
