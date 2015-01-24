#include "GUI.hpp"
#include "globals.hpp"
GUI::GUI(sf::Sprite* sprite)
{
	mySprite = sprite;
	timeoutClock = sf::Clock();
	timeoutClock.restart();
	mySprite->setPosition(10,screenHeight-30);
}
void GUI::setTimeout(int seconds)
{
	timeoutClock.restart();
	timeoutSeconds = seconds;
	timeBuff = 0;
}
void GUI::applyTimeBufff(float seconds)
{
	timeBuff += seconds;
}

void GUI::update (sf::Time deltaTime) {
	sf::Int32 currTime = globalClock.getElapsedTime().asMilliseconds();
	float elapsedSeconds = (timeoutClock.getElapsedTime().asSeconds()+timeBuff);
	float progress = 1-(elapsedSeconds / timeoutSeconds);
	//TODO: use min, max
	if (progress > 1)
	{
		progress = 1;
	}
	if (progress < 0)
	{
		progress = 0;
	}
	int width = (progress* (screenWidth-20));
	mySprite->setTextureRect(sf::IntRect(0, 0, width, 20));
	if (mySprite != 0)
	{
		window.draw(*mySprite);
	}
	for (int i = 0;i < coins;i++)
	{
		//TODO: Draw coins
	}
}

void GUI::resetCoins()
{
	coins = 0;
}

void GUI::addCoins(int amount)
{
	coins += amount;
}