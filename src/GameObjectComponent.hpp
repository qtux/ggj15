/*
 * GameObjectComponent.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */
#pragma once
//#include "GameObject.hpp"
class GameObject;
#include <SFML/System.hpp>

class GameObjectComponent {
public:
	GameObjectComponent(GameObject* object);
	virtual ~GameObjectComponent();
	virtual  void update(sf::Time deltaTime);
protected:
	GameObject* owner;
};
