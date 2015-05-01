#include "Highscore.hpp"
#include <fstream>
#include <sstream>
#include "global.hpp"

Highscore::Highscore(unsigned int levelNumber, const sf::Vector2f& size):
	levelNumber(levelNumber)
{
	_font.loadFromFile(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf");
	_textRectOne.setOutlineColor(sf::Color::White);
	_textRectOne.setOutlineThickness(2);
	_textRectOne.setPosition(30, 30);
	_textRectOne.setSize(sf::Vector2f(size.x - 60, size.y - 60));
	_textRectOne.setFillColor(sf::Color(0, 0, 250, 50));
	
	_textRectTwo.setOutlineColor(sf::Color::White);
	_textRectTwo.setOutlineThickness(1);
	_textRectTwo.setPosition(30, 99);
	_textRectTwo.setSize(sf::Vector2f(size.x - 60, 15));
	_textRectTwo.setFillColor(sf::Color(0, 0, 250, 50));
	
	int charSize = 10;
	_speech.setFont(_font);
	_speech.setColor(sf::Color(255,255,255));
	_speech.setCharacterSize(charSize);
	_speech.setStyle(sf::Text::Bold);
}

void Highscore::draw(sf::RenderTarget& target)
{
	// draw background
	target.draw(_textRectOne);
	target.draw(_textRectTwo);
	
	// draw level number
	_speech.setPosition(sf::Vector2f(45,50));
	_speech.setString(" Highscore level " + std::to_string(levelNumber));
	target.draw(_speech);
	
	// draw table header
	_speech.setPosition(sf::Vector2f(45,80));
	_speech.setString("Points         Trials            Coins         Time");
	target.draw(_speech);
	
	// show up to 8 old entries
	for (auto i = 0; i < std::min<std::vector<HighscoreRow>::size_type>(_rows.size(), 8); ++i)
	{
		int points = ((1.0+_rows[i].coins)/((float)_rows[i].kills+1)) * ((_rows[i].time+1)/_rows[i].maxtime) * 1000;
		_speech.setPosition(sf::Vector2f(45,100+(i * 30)));
		_speech.setString(std::to_string(points));
		target.draw(_speech);
		_speech.setPosition(sf::Vector2f(112,100+(i * 30)));
		_speech.setString(std::to_string(_rows[i].kills));
		target.draw(_speech);
		_speech.setPosition(sf::Vector2f(184,100+(i * 30)));
		_speech.setString(std::to_string(_rows[i].coins));
		target.draw(_speech);
		_speech.setPosition(sf::Vector2f(247,100+(i * 30)));
		_speech.setString(std::to_string((int)_rows[i].time)+"s");
		target.draw(_speech);
	}
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
