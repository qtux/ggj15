#include "GUI.hpp"
#include "globals.hpp"
GUI::GUI()
{
	timeSprite = new sf::Sprite();
	timeSprite->setTexture(textureManager.timeBarTexture);
	timeSprite->setPosition(10,screenHeight-30);

	coinSprite = new sf::Sprite();
	coinSprite->setTexture(textureManager.itemsTexture);
	coinSprite->setTextureRect(sf::IntRect(0,80,16,16));

	keySprite = new sf::Sprite();
	keySprite->setTexture(textureManager.itemsTexture);
	keySprite->setTextureRect(sf::IntRect(0,32,16,16));

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
	if (progress <= 0)
	{
		progress = 0;
		//sceneManager.restartLevel();
	}
	int width = (progress* (screenWidth-20));
	timeSprite->setTextureRect(sf::IntRect(width - int(elapsedSeconds) % 46, 0, width, 20));
	window.draw(*timeSprite);

	for (int i = 0;i < coins;i++)
	{
		//TODO: Draw coins
		coinSprite->setPosition(screenWidth-25,10+(i*16));
		window.draw(*coinSprite);
	}

	for (int i = 0;i < keys;i++)
	{
		//TODO: Draw coins
		keySprite->setPosition(i*16+40,screenHeight-50);
		window.draw(*keySprite);
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

void GUI::resetKeys()
{
	keys = 0;
}

void GUI::addKeys(int amount)
{
	keys += amount;
}
