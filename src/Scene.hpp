/*
 * Scene.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <vector>

class GameObject;

class Scene {
public:
	Scene();
	virtual ~Scene();

	GameObject* getTile(int x, int y);
	void setTile(GameObject* obj, int x, int y);

private:
	int sizeX, sizeY;
	std::vector<GameObject*> gameBoard;
};

#endif /* SCENE_HPP_ */
