#pragma once

#include "GameObject.hpp"
#include <ctime>

class GUI : public GameObject
{
public:
	GUI(sf::Sprite* sprite);
	void setTimeout(int seconds);
	void applyTimeBufff(float progress);
	virtual void update(sf::Time deltaTime);
private:
	sf::Clock timeoutClock;
	float timeoutSeconds;
	float timeBuff;
};
