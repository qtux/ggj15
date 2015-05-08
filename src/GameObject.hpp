/*
 * Scene.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */
#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class Level;

namespace sf{
	class Sprite;
}

class GameObject {

public:
	sf::Sprite * mySprite;
	
	virtual bool intersects(const GameObject& cmp) const;
	virtual bool intersects(const sf::Vector2f &testPos, const GameObject& cmp) const;
	
	const sf::Vector2f& getPosition() const;
	
	void setPosition(float x, float y);
	void setPosition(const sf::Vector2f &a);
	
	void checkTilesCollision(sf::Vector2f &testPos, const sf::Vector2f &previousPos, int chkColl[2]);

	int getWidth() const;
	
	int getHeight() const;

	//GameObject() {};

	virtual void update(sf::Time deltaTime) = 0;
	virtual void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader) = 0;
	
	virtual ~GameObject() {};

protected:
	Level* level;
	
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
