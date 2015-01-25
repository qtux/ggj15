#include "GUI.hpp"
#include "globals.hpp"
GUI::GUI()
{
	timeSprite = new sf::Sprite();
	timeSprite->setTexture(textureManager.timeBarTexture);
	timeSprite->setPosition(10, gridHeight-20);

	coinSprite = new sf::Sprite();
	coinSprite->setTexture(textureManager.itemsTexture);
	coinSprite->setTextureRect(sf::IntRect(0,80,16,16));

	keySprite = new sf::Sprite();
	keySprite->setTexture(textureManager.itemsTexture);
	keySprite->setTextureRect(sf::IntRect(0,32,16,16));

	timeoutClock = sf::Clock();
	timeoutClock.restart();
	coins=0;
	keys=0;
	pauseOffset = 0;
	loosed = false;
	lastEnable = false;
	smallTime = false;
	buffFactor = 20;
}
void GUI::setTimeout(int seconds)
{
	timeoutClock.restart();
	timeoutSeconds = seconds;
	timeBuff = 0;
}
void GUI::setTimebuffFactor(float factor)
{
	buffFactor = factor;
}
void GUI::applyTimeBufff(float seconds)
{
	timeBuff += seconds*buffFactor;
}

void GUI::update (sf::Time deltaTime) {
	if (sceneManager.getCurrentScene().textBox->enabled()){
		if (!lastEnable)
		{
			pauseOffset += timeoutClock.getElapsedTime().asSeconds();
		}
		timeoutClock.restart();
		lastEnable = true;
	}
	else
	{
		lastEnable = false;
	}
	sf::Int32 currTime = globalClock.getElapsedTime().asMilliseconds();
	float elapsedSeconds = (timeoutClock.getElapsedTime().asSeconds()+timeBuff+pauseOffset);
	float progress = 1 - (elapsedSeconds / timeoutSeconds);
	//TODO: use min, max
	if (progress > 1)
	{
		progress = 1;
	}
	if (progress <= 0)
	{
		progress = 0;
		if (!loosed){
			sceneManager.getCurrentScene().textBox->triggerText("loose");
		}
		loosed = true;
		if (!sceneManager.getCurrentScene().textBox->enabled())
		{
			sceneManager.restartLevel();
		}
	}
	if (progress < 0.3)
	{
		if (!smallTime)
		{
			sceneManager.getCurrentScene().textBox->triggerText("smalltime");
		}
		smallTime = true;
	}
	int height = (1 - progress) * (gridHeight);
	timeSprite->setPosition(-15 , height);
	timeSprite->setTextureRect(sf::IntRect(0, int(elapsedSeconds) % 45 + 2 * height, 10, gridHeight - height));
	window.draw(*timeSprite);

	for (int i = 0;i < coins;i++)
	{
		//TODO: Draw coins
		coinSprite->setPosition(gridWidth, - i * 16);
		window.draw(*coinSprite);
	}

	for (int i = 0;i < keys;i++)
	{
		//TODO: Draw coins
		keySprite->setPosition(gridWidth, gridHeight - 42 + i*16);
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
