#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
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
	Highscore(int level);
	virtual ~Highscore();
	void update(sf::Time deltaT);
	void save();
	void load();
private:
	int level;
	bool  actionPressed;
	std::vector<HighscoreRow> rows;
};