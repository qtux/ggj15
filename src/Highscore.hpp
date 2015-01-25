#pragma once
#include <SFML/Graphics.hpp>
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
};