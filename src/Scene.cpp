/*
 * Scene.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#include "Scene.hpp"
#include "Tile.hpp"
#include "GUI.hpp"

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

void Scene::switchLargeTile(int x1, int y1, int x2, int y2)
{
	GameObject* tmpObj;
	int startX1 = x1*largeTileSizeX;
	int startY1 = y1*largeTileSizeY;
	int startX2 = x2*largeTileSizeX;
	int startY2 = y2*largeTileSizeY;

	for (int x=0;x<largeTileSizeX;x++)
	{
		for (int y=0;largeTileSizeY;y++)
		{
			tmpObj = getTile(startX1+x, startY1+y);
			setTile(getTile(startX2+x, startY2+y), startX1+x, startY1+y);
			setTile(tmpObj, startX2+x, startY2+y);
		}
	}
}

void Scene::update(sf::Time deltaT)
{
	/*for (std::vector<GameObject*>::iterator it = gameBoard.begin();it != gameBoard.end(); it++)
	{
		(*it)->update(deltaT);
		std::cout << (*it) << std::endl;
	}*/
	for(auto& obj: gameBoard) {
		obj->update(deltaT);
	}
	player->update(deltaT);
	if (gui != 0)
	{
		gui->update(deltaT);
	}
}
