/*
 * Scene.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */
#pragma once
#include "GameObjectComponent.hpp"
#include <SFML/System.hpp>
#include <vector>

class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	void update(sf::Time deltaT);
	template<class t> t addComponent(){
		if (componentExits<t>())
		{
			return null;
		}
		t* result = new t(this);
		components.insert(result);
		return result;
	}
	template<class t> bool componentExits(){
		int size = components.size();
		for(int i = 0;i < size;++i)
		{
			if (dynamic_cast<t*>(components[i]))
			{
				return true;
			}
		}
		return false;
	}
	template<class t> t getComponent(){
		int size = components.size();
		for(int i = 0;i < size;++i)
		{
			if (dynamic_cast<t*>(components[i]))
			{
				return components[i];
			}
		}
		return null;
	}
	template<class t> t removeComponent(){
		vector<GameObjectComponent*>::iterator it = components.begin();
		for( ; it != components.end(); it++)
		{
			if (dynamic_cast<t*>(*it))
			{
				it = components.erase(it);
			}
		}
	}

private:
	std::vector<GameObjectComponent*> components;
};
