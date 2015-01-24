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
#include "GUI.hpp"
#include "Item.hpp"

class Scene {
public:
	Scene();
	virtual ~Scene();

	GameObject* getTile(int x, int y);
	void setTile(GameObject* obj, int x, int y);
	void setGUI(GUI* obj);
	void switchLargeTile(int x1, int y1, int x2, int y2);
	void update(sf::Time deltaT);
	void leave();
	// hack hier, ne doch nicht xD
	const static int sizeX = 8;
	const static int sizeY = 6;
	const static int largeTileSizeX = 6;
	const static int largeTileSizeY = 6;
	GameObject * player;
	GUI* gui;
	
	const std::vector<GameObject*> getGameBoard() const;
/*private:
	std::vector<GameObject*> gameBoard;*/
	sf::Vector2i startPos;
	sf::Vector2i portalPos;
	std::vector<Item> items;
private:
	std::vector<GameObject*> gameBoard;
};

#endif /* SCENE_HPP_ */
