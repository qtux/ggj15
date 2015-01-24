#include "GUI.hpp"
#include "globals.hpp"

GUI::GUI(sf::Sprite* sprite)
{
	mySprite = sprite;
	timeoutClock = sf::Clock();
	timeoutClock.restart();
}
void GUI::setTimeout(int seconds)
{
	timeoutClock.restart();
	timeoutSeconds = seconds;
}
void GUI::update (sf::Time deltaTime) {
	sf::Int32 currTime = globalClock.getElapsedTime().asMilliseconds();
	float elapsedSeconds = timeoutClock.getElapsedTime().asSeconds();
	int width = 0;//max(elapsedSeconds / timeoutSeconds,1)
	mySprite->setTextureRect(sf::IntRect(0, screenHeight-20, screenWidth, 30));
	if (mySprite != 0)
	{
		window.draw(*mySprite);
	}
}
