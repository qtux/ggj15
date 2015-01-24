/*
 * Scene.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <SFML/System.hpp>
#include <vector>
#include "GameObject.hpp"

class Scene {
public:
	Scene();
	virtual ~Scene();

	GameObject* getTile(int x, int y);
	void setTile(GameObject* obj, int x, int y);
	void switchLargeTile(int x1, int y1, int x2, int y2);
	void update(sf::Time deltaT);

private:
	int sizeX, sizeY, largeTileSizeX, largeTileSizeY;
	std::vector<GameObject*> gameBoard;
};

#endif /* SCENE_HPP_ */
