#pragma once

#include "GameObject.hpp"

class Item : public GameObject
{
public:
	Item(sf::Sprite* sprite,int x,int y,int w,int h);
	virtual ~Item();

	virtual void update(sf::Time deltaTime);
	virtual void applyEffect();
private:
	int drawX;
	int drawY;
	int drawW;
	int drawH;
	float offset;
	sf::Clock waveClock;
};
