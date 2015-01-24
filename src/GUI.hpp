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
	void addCoins(int amount);
private:
	sf::Clock timeoutClock;
	sf::Sprite* timeSprite;
	sf::Sprite* coinSprite;
	sf::Texture timeBarTexture;
	sf::Texture coinTexture;
	float timeoutSeconds;
	float timeBuff;
	int coins;
};
