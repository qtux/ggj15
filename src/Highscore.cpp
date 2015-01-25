#include "Highscore.hpp"
#include <fstream>
#include <sstream>
#include <cmath>
#include "globals.hpp"

Highscore::Highscore(int level)
{
	this->level = level;
	actionPressed = false;
}
Highscore::~Highscore()
{
}
void Highscore::update(sf::Time deltaT)
{
	if (!input[4] && actionPressed)
	{
		sceneManager.nextLevel();
	}
	actionPressed = input[4];
}


void Highscore::save()
{
	std::vector<std::string> hsFile;
	std::ifstream infile("hs.txt");
	std::string line;
	while (std::getline(infile, line))
	{
		hsFile.push_back(line);
	}
	infile.close();

	std::stringstream hsLineStr;
	hsLineStr << sceneManager.getCurrentScene().gui->coins << " ";
	hsLineStr << sceneManager.getCurrentScene().gui->timeLeft() << " ";
	hsLineStr << sceneManager.getCurrentScene().gui->timeoutSeconds;
	
	std::ofstream out("hs.txt");
	out << hsLineStr.str() << "\n";
	for (std::vector<std::string>::iterator lineIt = hsFile.begin() ; lineIt != hsFile.end() ; lineIt++)
	{
		out << (*lineIt) << "\n";
	}
	out.close();

}

void Highscore::load()
{
	sf::Font font;
	font.loadFromFile(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf");

	// Text TEST
	int charSize = 10;
	
	sf::Text speech;
	speech.setFont(font);
	speech.setColor(sf::Color(255,255,255));
	speech.setCharacterSize(charSize);
	speech.setPosition(sf::Vector2f(40,50));
	speech.setStyle(sf::Text::Bold);
	speech.setString("Highscore");
	window.draw(speech);
	speech.setPosition(sf::Vector2f(40,80));
	speech.setStyle(sf::Text::Bold);
	speech.setString("Points   Kills      Coins       Time");
	window.draw(speech);

	
	sf::RectangleShape textRect;
	textRect.setOutlineColor(sf::Color::White);
	textRect.setOutlineThickness(2);
	textRect.setPosition(30, 30);
	textRect.setSize(sf::Vector2f(gridWidth - 30, gridHeight - 30));
	textRect.setFillColor(sf::Color(0, 0, 250, 50));
	window.draw(textRect);
	
	int rows = 0;
	std::ifstream infile(std::string(PATH) + "hs.hus");
	std::string line;

	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		int coins,time,kills,maxtime;
		iss >> coins;
		iss >> time;
		iss >> maxtime;
		iss >> kills;
		int points = ((1+coins)/kills) * (time/time*maxtime) * 1000;
		speech.setPosition(sf::Vector2f(40,100+(rows * 30)));
		speech.setString(std::to_string(points));
		speech.setPosition(sf::Vector2f(70,100+(rows * 30)));
		speech.setString(std::to_string(kills));
		speech.setPosition(sf::Vector2f(70,100+(rows * 30)));
		speech.setString(std::to_string(coins));
		speech.setPosition(sf::Vector2f(100,100+(rows * 30)));
		speech.setString(std::to_string(time));

		window.draw(speech);
		rows++;
	}
}
