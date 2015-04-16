#pragma once

#include <vector>
#include <string>
#include <map>
#include "Level.hpp"
#include <SFML/Graphics.hpp>

class SceneManager {
private:
	// SceneLoader loader;
	Level scene;
	std::map<sf::Uint32, std::vector<sf::Vector2i>*> colorToTilePositionMap;
	std::map<sf::Uint32, bool> walkableTileState;
	
	sf::Vector2i getRandomGrassPosition();
	int currentLevelNumber;
	sf::Uint32 createColorKey(sf::Color color);
	std::vector<sf::Vector2i>* checkNeighbours(sf::Uint32 color, int x, int y, sf::Image levelImg);
public:
	SceneManager();
	void showScene(std::string sceneName);
	void update(sf::Time deltaT);
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	void loadScene(std::string name);
	void processEditMode();
	void nextLevel();
	void nextLevel(int number);
	void restartLevel();
	int getCurrentLevelNumber();
	int restards;
	sf::Vector2i getTilePosition(sf::Uint32 color, int x, int y, sf::Image levelImg);
	
	Level& getCurrentScene();
};
