#include "SceneManager.hpp"

SceneManager::SceneManager(): currentScene(Scene()) {
}

void SceneManager::showScene(std::string sceneName) {
	// load scene if not existent and set currentScene
}

void SceneManager::update(sf::Time deltaT) {
	currentScene.update(deltaT);
}
