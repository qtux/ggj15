#include "SceneManager.hpp"

SceneManager::SceneManager(Game* game): game(game), currentScene(game) {
}

void SceneManager::showScene(std::string sceneName) {
	// load scene if not existent and set currentScene
}

void SceneManager::update(sf::Time deltaT) {
	currentScene.update(deltaT);
}
