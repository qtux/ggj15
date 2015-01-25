#pragma once

#include "GameObject.hpp"

class Item : public GameObject
{
public:
	Item(sf::Sprite* sprite,int x,int y,int w,int h);
	virtual ~Item();

	virtual void update(sf::Time deltaTime);
	virtual bool applyEffect();
	
	bool collectable;
	bool blocksPath;
	int drawX;
	int drawY;
	int drawW;
	int drawH;
private:
	float offset;
	sf::Clock waveClock;
};
