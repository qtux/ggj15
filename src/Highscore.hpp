#pragma once
#include "globals.hpp";
#include <SFML/Graphics.hpp>
class Highscore
{
public:
	Highscore(int level);
	void update(sf::Time deltaT);
	void save();
	void load();
private:
	int level;
	bool  actionPressed;
};