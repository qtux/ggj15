/*
 * Scene.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#include "Scene.hpp"
#include "Tile.hpp"
#include "globals.hpp"
#include <iostream>
#include "GUI.hpp"
#include <math.h>
#include "globals.hpp"
#include "KeyItem.hpp"

Scene::Scene() {
	// TODO Auto-generated constructor stub
	gameBoard.resize(sizeX * sizeY * largeTileSizeX * largeTileSizeY);

}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}


GameObject* Scene::getTile(int x, int y)
{
	if (x + y*sizeX < (int)gameBoard.size())
	{
		return gameBoard[x + y * sizeX * largeTileSizeX];
	}
	return 0;
}


void Scene::setTile(GameObject* obj, int x, int y)
{
	gameBoard[x + y * sizeX * largeTileSizeX] = obj;
}

void Scene::setGUI(GUI* obj)
{
	gui = obj;
}

const std::vector<GameObject*> &Scene::getGameBoard() const
{
	return gameBoard;
}

void Scene::switchLargeTile(int x1, int y1, int x2, int y2)
{
	int startX1 = x1*largeTileSizeX;
	int startY1 = y1*largeTileSizeY;
	int startX2 = x2*largeTileSizeX;
	int startY2 = y2*largeTileSizeY;

	for (int x=0;x<largeTileSizeX;x++)
	{
		for (int y=0;y<largeTileSizeY;y++)
		{
			sf::Vector2f tmpPos = getTile(startX1+x, startY1+y)->getPosition();
			sf::Vector2f tmpPos2 = getTile(startX2+x, startY2+y)->getPosition();
			getTile(startX2+x, startY2+y)->setPosition(tmpPos.x, tmpPos.y);
			getTile(startX1+x, startY1+y)->setPosition(tmpPos2.x, tmpPos2.y);
		}
	}
}

void Scene::update(sf::Time deltaT)
{
//	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
//	{
//		sf::Vector2i globalPosition = sf::Mouse::getPosition(window);
//
//		sf::Vector2f localPosition;
//		localPosition.x = 1.f*globalPosition.x/(Tile::pixelSizeX*Tile::tileScaleFactor);
//		localPosition.y = 1.f*globalPosition.y/(Tile::pixelSizeY*Tile::tileScaleFactor);
//		std::cout<<localPosition.x<<", "<<localPosition.y<<std::endl;
//	}
	/*for (std::vector<GameObject*>::iterator it = gameBoard.begin();it != gameBoard.end(); it++)
	{
		(*it)->update(deltaT);
		std::cout << (*it) << std::endl;
	}*/
	for(auto& obj: gameBoard) {
		obj->update(deltaT);
	}

	for(auto& obj: items) {
		obj->update(deltaT);
	}
	player->update(deltaT);
	if (gui != 0)
	{
		gui->update(deltaT);
	}
	
	sf::Font font;
	font.loadFromFile(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf");
	
	sf::Text level;
	level.setFont(font);
	level.setPosition(screenWidth - 30, screenHeight - 70);
	level.setString(std::to_string(sceneManager.getCurrentLevelNumber()));
	window.draw(level);
	
	for(std::vector<Item*>::iterator itIt = items.begin() ; itIt != items.end() ; itIt++) {
		if (player->intersects(**itIt))
		{
			(*itIt)->applyEffect();
			itIt = items.erase(itIt);
		} 
	}
	
	/*
	// Text TEST
	sf::Vector2f textPos(32.0f, 32.0f);
	* int charSize = 30;
	
	sf::Text speech;
	speech.setFont(font);
	
	speech.setColor(sf::Color(210, 210, 255));
	speech.setCharacterSize(charSize);
	speech.setPosition(textPos);
	
	sf::RectangleShape textRect;
	textRect.setOutlineColor(sf::Color::Blue);
	textRect.setOutlineThickness(5);
	textRect.setPosition(textPos.x - 5, textPos.y - 5);
	textRect.setSize(sf::Vector2f(screenWidth - 2* textPos.x + 10, 2 * charSize + 30));
	textRect.setFillColor(sf::Color(0, 0, 250, 50));
	window.draw(textRect);
	
	// zu Anfang:
	speech.setStyle(sf::Text::Bold);
	speech.setString("Oh no...");
	speech.setStyle(sf::Text::Regular);
	speech.setString("The time machine is broken, doggie!");
	speech.setString("...");
	speech.setString("What do we do now?");
	speech.setColor(sf::Color(210, 255, 210));
	speech.setString("SQOLRK");
	speech.setColor(sf::Color(210, 210, 255));
	
	// Zeit wird knapp:
	speech.setColor(sf::Color(210, 255, 210));
	speech.setString("LURMK");
	speech.setColor(sf::Color(210, 210, 255));
	speech.setString("You are right we should hurry. The pizza is going cold.");
	
	// Key aufgesammelt (erstes Level):
	speech.setColor(sf::Color(210, 210, 210));
	speech.setString("A key to another dimension!");
	speech.setColor(sf::Color(210, 210, 255));
	
	// Uhr aufgesammelt (erstes Level):
	speech.setColor(sf::Color(210, 210, 210));
	speech.setString("When do we do now?");
	speech.setColor(sf::Color(210, 210, 255));
	
	// Ziel erreicht, kein Key (erstes Level):
	speech.setString("We need a key for this dimension hole!");
	
	// Ziel erreicht (erstes Level):
	speech.setString("Do you want to leave, Doggie?");
	speech.setColor(sf::Color(210, 255, 210));
	speech.setString("Frravt");
	speech.setColor(sf::Color(210, 210, 255));
	
	// Spielende:
	schwarzer Bildschirm
	speech.setColor(sf::Color(210, 255, 210));
	speech.setString("SQOLRK");
	speech.setColor(sf::Color(210, 210, 255));
	schwarzer Bildschirm
	
	window.draw(speech);
	*/
}

void Scene::leave()
{
	int size = items.size();
	int keysInLevel = 0;
	for(int i = 0;i < size;i++)
	{
		if (dynamic_cast<KeyItem*>(items[i]))
		{
			keysInLevel++;
		}
	}
	if (keysInLevel > 0)
	{
		return;
	}
	gui->resetCoins();
	gui->resetKeys();
	sceneManager.nextLevel();
}
