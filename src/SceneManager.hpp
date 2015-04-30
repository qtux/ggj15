#pragma once

#include "Level.hpp"
#include <SFML/Graphics.hpp>

class SceneManager {
private:
	Level* scene;
	int currentLevelNumber;
public:
	SceneManager();
	void update(sf::Time deltaT, sf::RenderWindow& window);
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	void processEditMode();
	void nextLevel();
	void nextLevel(int number);
	void restartLevel();
	Level* getCurrentScene();
};
