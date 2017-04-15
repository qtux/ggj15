/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "Highscore.hpp"
#include <fstream>
#include <sstream>
#include "global.hpp"

Highscore::Highscore(unsigned int levelNumber, const sf::Vector2f& size):
	levelNumber(levelNumber)
{
	_textRectOne.setOutlineColor(sf::Color::White);
	_textRectOne.setOutlineThickness(2);
	_textRectOne.setPosition(30, 30);
	_textRectOne.setSize(sf::Vector2f(size.x - 60, size.y - 60));
	_textRectOne.setFillColor(sf::Color(0, 0, 250, 50));
	
	_textRectTwo.setOutlineColor(sf::Color::White);
	_textRectTwo.setOutlineThickness(1);
	_textRectTwo.setPosition(30, 95);
	_textRectTwo.setSize(sf::Vector2f(size.x - 60, 30));
	_textRectTwo.setFillColor(sf::Color(0, 0, 250, 50));
	
	int charSize = 32;
	_speech.setFont(gb::ressourceManager.getFont(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf"));
	_speech.setFillColor(sf::Color(255,255,255));
	_speech.setCharacterSize(charSize);
	_speech.setStyle(sf::Text::Bold);
	_speech.setPosition(sf::Vector2f(45,50));
	_speech.setOutlineColor(sf::Color::Black);
	_speech.setOutlineThickness(1.0f);
}

void Highscore::draw(sf::RenderTarget& target)
{
	// draw background
	target.draw(_textRectOne);
	target.draw(_textRectTwo);
	
	// draw table
	std::string text = "Highscore\tLevel " + std::to_string(levelNumber);
	text += "\nPoints\tTrials\tCoins\tTime";
	for (auto i = 0; i < std::min<std::vector<HighscoreRow>::size_type>(_rows.size(), 8); ++i)
	{
		int points = ((1.0+_rows[i].coins)/((float)_rows[i].kills+1)) * ((_rows[i].time+1)/_rows[i].maxtime) * 1000;
		text += "\n" + std::to_string(points);
		text += "\t\t" + std::to_string(_rows[i].kills);
		text += "\t\t" + std::to_string(_rows[i].coins);
		text += "\t\t" + std::to_string((int)_rows[i].time) + "s";
	}
	_speech.setString(text);
	target.draw(_speech);
}

void Highscore::save(unsigned int coins, unsigned int timeLeft, unsigned int timeoutSeconds, unsigned int restarts)
{
	std::vector<std::string> hsFile;
	std::ifstream infile(std::string(PATH) + "hs"+std::to_string(levelNumber)+".txt");
	std::string line;
	while (std::getline(infile, line))
	{
		hsFile.push_back(line);
	}
	infile.close();
	
	std::stringstream hsLineStr;
	hsLineStr << coins << " ";
	hsLineStr << timeLeft << " ";
	hsLineStr << timeoutSeconds << " ";
	hsLineStr << restarts;
	
	std::ofstream out(std::string(PATH) + "hs"+std::to_string(levelNumber)+".txt");
	out << hsLineStr.str() << "\n";
	for (std::vector<std::string>::iterator lineIt = hsFile.begin() ; lineIt != hsFile.end() ; lineIt++)
	{
		out << (*lineIt) << "\n";
	}
	out.close();
}

void Highscore::load()
{
	std::ifstream infile(std::string(PATH) + "hs"+std::to_string(levelNumber)+".txt");
	std::string line;
	
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		HighscoreRow row = HighscoreRow();
		iss >> row.coins;
		iss >> row.time;
		iss >> row.maxtime;
		iss >> row.kills;
		_rows.push_back(row);
	}
}
