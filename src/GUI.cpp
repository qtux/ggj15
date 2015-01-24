#include "GUI.hpp"
#include "globals.hpp"
GUI::GUI()
{
	timeBarTexture.loadFromFile(std::string(PATH) + "img/timeBar.png");
	timeBarTexture.setRepeated(true);
	timeSprite = new sf::Sprite();
	timeSprite->setTexture(timeBarTexture);
	timeSprite->setPosition(10,screenHeight-30);

	coinTexture.loadFromFile(std::string(PATH) + "img/items.png");
	coinSprite = new sf::Sprite();
	coinSprite->setTexture(coinTexture);
	coinSprite->setTextureRect(sf::IntRect(0,80,16,16));

	timeoutClock = sf::Clock();
	timeoutClock.restart();

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
	timeSprite->setTextureRect(sf::IntRect(0, 0, width, 20));
	window.draw(*timeSprite);

	for (int i = 0;i < coins;i++)
	{
		//TODO: Draw coins
		coinSprite->setPosition(screenWidth-16,10+(i*16));
		window.draw(*coinSprite);
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