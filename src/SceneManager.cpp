#include "SceneManager.hpp"
#include "Tile.hpp"
#include "global.hpp"

SceneManager::SceneManager():
	currentLevelNumber(-1),
	restards(0),
	scene(new Level())
{
	nextLevel();
}

void SceneManager::update(sf::Time deltaT, sf::RenderWindow& window) {
	//processEditMode();	// only for edit mode
	scene->update(deltaT, window);
}

void SceneManager::draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader) {
	//processEditMode();	// only for edit mode
	scene->draw(renderTarget);
}

void SceneManager::processEditMode()
{
	for (int key=4;key<13;key++)
	{
		if (gb::input[key] == true)
		{
			sf::Vector2i globalPosition = sf::Mouse::getPosition(gb::window);

			sf::Vector2f localPosition;
			localPosition.x = 1.f * globalPosition.x / (gb::pixelSizeX);
			localPosition.y = 1.f * globalPosition.y / (gb::pixelSizeY);
//			std::cout<<localPosition.x<<", "<<localPosition.y<<std::endl;


			sf::Vector2i tmpPos;
			tmpPos.x = (int)(localPosition.x);
			tmpPos.y = (int)(localPosition.y);

			GameObject* tmpTile = scene->getTile(tmpPos.x, tmpPos.y);
			Tile* newTile = new Tile();

			newTile->mySprite = tmpTile->mySprite;

			sf::Vector2i posInTexture = sf::Vector2i(0, 0); // grass
			newTile->mySprite->setTextureRect(sf::IntRect(posInTexture.x * gb::pixelSizeX, posInTexture.y * gb::pixelSizeY, gb::pixelSizeX, gb::pixelSizeY));
			scene->setTile(newTile,tmpPos.x, tmpPos.y);
		}
	}
}

void SceneManager::nextLevel(){
	currentLevelNumber++;
	delete scene;
	scene = new Level();
	scene->loadScene(std::string(PATH) + "levels/level"+std::to_string(currentLevelNumber));
	restards = 0;
}

void SceneManager::nextLevel(int number){
	currentLevelNumber = number;
	delete scene;
	scene = new Level();
	scene->loadScene(std::string(PATH) + "levels/level"+std::to_string(currentLevelNumber));
}

void SceneManager::restartLevel(){
	delete scene;
	scene = new Level();
	scene->loadScene(std::string(PATH) + "levels/level"+std::to_string(currentLevelNumber));
	restards++;
}

Level* SceneManager::getCurrentScene()
{
	return scene;
}

int SceneManager::getCurrentLevelNumber()
{
	return currentLevelNumber;
}
