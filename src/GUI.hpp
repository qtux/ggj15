#pragma once

#include "GameObject.hpp"
#include <ctime>

class GUI : public GameObject
{
public:
	GUI(sf::Sprite* timeSprite,sf::Sprite* coin);
	void setTimeout(int seconds);
	void applyTimeBufff(float progress);
	virtual void update(sf::Time deltaTime);
	void resetCoins();
	void addCoins(int amount);
private:
	sf::Clock timeoutClock;
	sf::Sprite* cointSprite;
	float timeoutSeconds;
	float timeBuff;
	int coins;
};
