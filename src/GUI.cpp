#include "GUI.hpp"
#include "globals.hpp"
GUI::GUI()
{
	timeSprite = new sf::Sprite();
	timeSprite->setTexture(gb::textureManager.timeBarTexture);
	timeSprite->setPosition(10, gb::gridHeight-20);

	coinSprite = new sf::Sprite();
	coinSprite->setTexture(gb::textureManager.itemsTexture);
	coinSprite->setTextureRect(sf::IntRect(0,80,16,16));

	keySprite = new sf::Sprite();
	keySprite->setTexture(gb::textureManager.itemsTexture);
	keySprite->setTextureRect(sf::IntRect(0,32,16,16));

	timeoutClock = sf::Clock();
	timeoutClock.restart();
	coins=0;
	keys=0;
	timeoutSeconds=0;
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
float GUI::timeLeft()
{
	float elapsedSeconds = (timeoutClock.getElapsedTime().asSeconds()+timeBuff+pauseOffset);
	float timeLeft = timeoutSeconds - elapsedSeconds;
	return timeLeft < 0 ? 0 : timeLeft;
}

void GUI::update (sf::Time deltaTime) {
	if (gb::sceneManager.getCurrentScene().textBox->enabled()){
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
			gb::sceneManager.getCurrentScene().textBox->triggerText("loose");
		}
		loosed = true;
		if (!gb::sceneManager.getCurrentScene().textBox->enabled())
		{
			gb::sceneManager.restartLevel();
		}
	}
	if (progress < 0.3)
	{
		if (!smallTime)
		{
			gb::sceneManager.getCurrentScene().textBox->triggerText("smalltime");
			gb::soundManager.playSound("sound/timeShort.ogg");
			
		}
		smallTime = true;
	}
	int height = (1 - progress) * (gb::gridHeight);
	timeSprite->setPosition(-15 , height);
	timeSprite->setTextureRect(sf::IntRect(0, int(elapsedSeconds) % 45 + 2 * height, 10, gb::gridHeight - height));
	gb::window.draw(*timeSprite);

	for (int i = 0;i < coins;i++)
	{
		//TODO: Draw coins
		coinSprite->setPosition(gb::gridWidth, i * 16);
		gb::window.draw(*coinSprite);
	}

	for (int i = 0;i < keys;i++)
	{
		//TODO: Draw coins
		keySprite->setPosition(gb::gridWidth, gb::gridHeight - 42 - i*16);
		gb::window.draw(*keySprite);
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
