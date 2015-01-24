/*
 * GameObject.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */
#pragma once
#include "GameObjectComponent.hpp"
#include <iostream>

GameObjectComponent::GameObjectComponent(GameObject* object) {
	owner = object;

}

GameObjectComponent::~GameObjectComponent() {
	// TODO Auto-generated destructor stub
}

void GameObjectComponent::update(sf::Time deltaTime) {
	// TODO Auto-generated destructor stub
}