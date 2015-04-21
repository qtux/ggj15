#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Level;

class HighscoreRow
{
public:
	int coins;
	float time;
	float maxtime;
	int kills;
};
class Highscore
{
public:
	Highscore(Level* level);
	void update(sf::Time deltaT);
	void save();
	void load();
private:
	Level* level;
	bool  actionPressed;
	std::vector<HighscoreRow> rows;
};
