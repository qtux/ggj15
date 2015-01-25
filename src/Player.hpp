#pragma once

#include "GameObject.hpp"
#include "globals.hpp"
#include <queue>

class Player : public GameObject
{
public:
	float animationStep;
	int direction;
	float doggieStep;
	
	virtual bool intersects(const GameObject& cmp);
	virtual bool intersects(const sf::Vector2f &testPos, const GameObject& cmp);

	Player(): animationStep(0.), direction(0),doggieStep(0.) {};

	virtual void update(sf::Time deltaTime);
	
	// TODO put doggieSprite in here and not in GameObject
	// TODO delete doggieSprites
	
private:
	std::queue<sf::Vector2f> positionQueue;
	std::queue<int> directionQueue;
	
};

const static int PlayerAnimState[] = {0, 1, 0, 2};
const static int DoggieAnimState[] = {0,2,1,3,1,4};

const static int slowFactor = 15;
