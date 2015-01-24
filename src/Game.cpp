#include "Game.hpp"

Game::Game(sf::RenderWindow* window): window(window), sceneManager(SceneManager(this)) {
}

void Game::update(sf::Time deltaT) {
	sceneManager.update(deltaT);
}
