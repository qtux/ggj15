/*
 * Scene.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */
#pragma once
#include <SFML/System.hpp>
#include <vector>

namespace sf{
	class Sprite;
}

class GameObject {

public:
	sf::Sprite * mySprite;
	sf::Sprite * doggieSprite;
	
	const sf::Vector2f& getPosition() const;
	
	void setPosition(int x, int y);
	void setPosition(float x, float y);
	
	int getWidth() const;
	
	int getHeight() const;

	//GameObject() {};

	virtual void update(sf::Time deltaTime) = 0;
	
	virtual ~GameObject() {};
	
/*
public:
	int xPos;
	int yPos;
	
	GameObject();
	virtual ~GameObject();
	void update(sf::Time deltaT);
	template<class t> t addComponent(){
		if (componentExits<t>())
		{
			return 0;
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
		return 0;
	}
	template<class t> t removeComponent(){
		std::vector<GameObjectComponent*>::iterator it = components.begin();
		for( ; it != components.end(); it++)
		{
			if (dynamic_cast<t*>(*it))
			{
				it = components.erase(it);
			}
		}
	}

private:
	std::vector<GameObjectComponent*> components;*/
};
