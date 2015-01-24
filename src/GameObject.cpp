/*
 * GameObject.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#include "GameObject.hpp"
#include <iostream>

GameObject::GameObject() {
	std::cout<<"GameObject created"<<std::endl;
	// TODO Auto-generated constructor stub

}

GameObject::~GameObject() {
	// TODO Auto-generated destructor stub
}

void GameObject::update(sf::Time deltaT)
{
	// TODO: implement
	int size = components.size();
	for(int i = 0;i < size;++i)
	{
		components[i]->update(deltaT);
	}
}