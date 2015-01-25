#pragma once
#include <SFML/Graphics.hpp>
#include <ctime>

class GUI
{
public:
	GUI();
	void setTimeout(int seconds);
	void applyTimeBufff(float progress);
	void update(sf::Time deltaTime);
	void resetCoins();
	void resetKeys();
	void addCoins(int amount);
	void addKeys(int amount);
	void setTimebuffFactor(float factor);
private:
	sf::Clock timeoutClock;
	sf::Sprite* timeSprite;
	sf::Sprite* coinSprite;
	sf::Sprite* keySprite;

	float timeoutSeconds;
	float timeBuff;
	float pauseOffset;
	bool lastEnable;
	bool loosed;
	bool smallTime;
	float buffFactor;
	int coins;
	int keys;
};
