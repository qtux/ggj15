/*
 * Scene.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#include "Scene.hpp"
#include "GameObject.hpp"

Scene::Scene() {
	// TODO Auto-generated constructor stub
	sizeX = 8;
	sizeY = 6;

	for (int x=0;x<sizeX;x++)
	{
		for (int y=0;y<sizeY;y++)
		{
			gameBoard.push_back(new GameObject());
		}
	}

}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}


GameObject* Scene::getTile(int x, int y)
{
	if (x + y*sizeX < (int)gameBoard.size())
	{
		return gameBoard[x + y*sizeX];
	}
	return 0;
}


void Scene::setTile(GameObject* obj, int x, int y)
{
	gameBoard[x + y*sizeX] = obj;
}
