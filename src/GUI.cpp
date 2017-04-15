/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "GUI.hpp"
#include "global.hpp"
#include "Level.hpp"

GUI::GUI(Level* level):
	level(level)
{
	timeSprite = new sf::Sprite();
	timeSprite->setTexture(gb::ressourceManager.getTexture(std::string(PATH) + "img/timeBar.png" , true));
	timeSprite->setPosition(10, gb::gridHeight-20);

	coinSprite = new sf::Sprite();
	coinSprite->setTexture(gb::ressourceManager.getTexture(std::string(PATH) + "img/items.png", false));
	coinSprite->setTextureRect(sf::IntRect(3*32, 2*32, 32, 32));

	keySprite = new sf::Sprite();
	keySprite->setTexture(gb::ressourceManager.getTexture(std::string(PATH) + "img/items.png", false));
	keySprite->setTextureRect(sf::IntRect(1*32, 2*32, 32, 32));

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
	
	// level number
	levelNumber.setFont(gb::ressourceManager.getFont(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf"));
	levelNumber.setPosition(gb::gridWidth + 2, gb::gridHeight - 32);
	levelNumber.setString(std::to_string(level->levelNumber));
}
void GUI::setTimeout(int seconds)
{
	timeoutClock.restart();
	timeoutSeconds = seconds;
	timeBuff = 0;
}
float GUI::getTimebuffFactor() const
{
	return buffFactor;
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
	if (level->textBox->enabled()){
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
			level->textBox->triggerText("loose");
		}
		loosed = true;
		if (!level->textBox->enabled())
		{
			level->reset();
		}
	}
	if (progress < 0.3)
	{
		if (!smallTime)
		{
			level->textBox->triggerText("smalltime");
			gb::soundManager.playSound("sound/timeShort.ogg");
			
		}
		smallTime = true;
	}
	int height = (1 - progress) * (gb::gridHeight);
	timeSprite->setPosition(-15 , height);
	timeSprite->setTextureRect(sf::IntRect(0, int(elapsedSeconds) % 45 + 2 * height, 10, gb::gridHeight - height));
}

void GUI::draw(sf::RenderTarget &renderTarget)
{
	renderTarget.draw(*timeSprite);
	for (int i = 0;i < coins;i++)
	{
		//TODO: Draw coins
		coinSprite->setPosition(gb::gridWidth, i * 32);
		renderTarget.draw(*coinSprite);
	}
	for (int i = 0;i < keys;i++)
	{
		//TODO: Draw coins
		keySprite->setPosition(gb::gridWidth, gb::gridHeight - 58 - i * 32);
		renderTarget.draw(*keySprite);
	}
	renderTarget.draw(levelNumber);
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

// TODO insert me
/*
	
*/
