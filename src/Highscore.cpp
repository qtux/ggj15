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
	showOutline = false;
	if (!input[4] && actionPressed)
	{
		sceneManager.nextLevel();
	}
	actionPressed = input[4];

	sf::Font font;
	font.loadFromFile(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf");

	// Text TEST
	int charSize = 10;
	sf::RectangleShape textRect;
	textRect.setOutlineColor(sf::Color::White);
	textRect.setOutlineThickness(2);
	textRect.setPosition(30, 30);
	textRect.setSize(sf::Vector2f(gridWidth - 30, gridHeight - 60));
	textRect.setFillColor(sf::Color(0, 0, 250, 50));
	window.draw(textRect);
	textRect.setOutlineThickness(1);
	textRect.setPosition(30, 99);
	textRect.setSize(sf::Vector2f(gridWidth - 30, 15));
	window.draw(textRect);
	sf::Text speech;
	speech.setFont(font);
	speech.setColor(sf::Color(255,255,255));
	speech.setCharacterSize(charSize+1);
	speech.setPosition(sf::Vector2f(45,50));
	speech.setStyle(sf::Text::Bold);
	speech.setString(" Highscore level " + std::to_string(level));
	speech.setCharacterSize(charSize);
	window.draw(speech);
	speech.setPosition(sf::Vector2f(45,80));
	speech.setStyle(sf::Text::Bold);
	speech.setString("Points         Trials            Coins         Time");
	window.draw(speech);

	


	int size = rows.size();
	if (size > 8)
	{
		size = 8;
	}
	for(int i = 0;i < size;i++){
		HighscoreRow row = rows[i];
		int points = ((1.0+row.coins)/((float)row.kills+1)) * ((row.time+1)/row.maxtime) * 1000;
		speech.setPosition(sf::Vector2f(45,100+(i * 30)));
		speech.setString(std::to_string(points));
		window.draw(speech);
		speech.setPosition(sf::Vector2f(112,100+(i * 30)));
		speech.setString(std::to_string(row.kills));
		window.draw(speech);
		speech.setPosition(sf::Vector2f(184,100+(i * 30)));
		speech.setString(std::to_string(row.coins));
		window.draw(speech);
		speech.setPosition(sf::Vector2f(247,100+(i * 30)));
		speech.setString(std::to_string((int)row.time)+"s");
		window.draw(speech);
	}
}


void Highscore::save()
{

	std::vector<std::string> hsFile;
	std::ifstream infile(std::string(PATH) + "hs"+std::to_string(level)+".txt");
	std::string line;
	while (std::getline(infile, line))
	{
		hsFile.push_back(line);
	}
	infile.close();

	std::stringstream hsLineStr;
	hsLineStr << sceneManager.getCurrentScene().gui->coins << " ";
	hsLineStr << sceneManager.getCurrentScene().gui->timeLeft() << " ";
	hsLineStr << sceneManager.getCurrentScene().gui->timeoutSeconds << " ";
	hsLineStr << sceneManager.restards;
	
	std::ofstream out(std::string(PATH) + "hs"+std::to_string(level)+".txt");
	out << hsLineStr.str() << "\n";
	for (std::vector<std::string>::iterator lineIt = hsFile.begin() ; lineIt != hsFile.end() ; lineIt++)
	{
		out << (*lineIt) << "\n";
	}
	out.close();
}

void Highscore::load()
{
	
	std::ifstream infile(std::string(PATH) + "hs"+std::to_string(level)+".txt");
	std::string line;

	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		HighscoreRow row = HighscoreRow();
		iss >> row.coins;
		iss >> row.time;
		iss >> row.maxtime;
		iss >> row.kills;
		rows.push_back(row);
	}
}