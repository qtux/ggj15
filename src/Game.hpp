#pragma once

#include <SFML/Graphics.hpp>
#include "SceneManager.hpp"

class Game {
private:
	sf::RenderWindow* window;
public:
	Game(sf::RenderWindow* window);
	void update(sf::Time deltaT);
	SceneManager sceneManager;
};
