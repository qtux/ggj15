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
	void draw(sf::RenderTarget& target);
	void save();
	void load();
	bool nextLevel;
private:
	Level* level;
	bool  actionPressed;
	std::vector<HighscoreRow> rows;
};
