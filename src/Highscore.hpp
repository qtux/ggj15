#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Level;

class Highscore
{
public:
	struct HighscoreRow
	{
		unsigned int coins;
		float time;
		float maxtime;
		unsigned int kills;
	};
	Highscore(unsigned int levelNumber, const sf::Vector2f& size);
	void draw(sf::RenderTarget& target);
	void save(unsigned int coins, unsigned int timeLeft, unsigned int timeoutSeconds, unsigned int restarts);
	void load();
	const unsigned int levelNumber;
private:
	sf::RectangleShape _textRectOne;
	sf::RectangleShape _textRectTwo;
	sf::Text _speech;
	std::vector<HighscoreRow> _rows;
};

// TODO make this resolution independent
