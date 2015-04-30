#include "SceneManager.hpp"
#include "Tile.hpp"
#include "global.hpp"

SceneManager::SceneManager():
	currentLevelNumber(-1),
	scene(new Level(0))
{}

void SceneManager::update(sf::Time deltaT, sf::RenderWindow& window) {
	scene->update(deltaT, window);
}

void SceneManager::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader) {
	scene->draw(renderTarget, false);
}

void SceneManager::nextLevel(){
	currentLevelNumber++;
	delete scene;
	scene = new Level(currentLevelNumber);
}

void SceneManager::nextLevel(int number){
	currentLevelNumber = number;
	delete scene;
	scene = new Level(currentLevelNumber);
}

void SceneManager::restartLevel(){
	scene->reset();
}

Level* SceneManager::getCurrentScene()
{
	return scene;
}

int SceneManager::getCurrentLevelNumber()
{
	return currentLevelNumber;
}
