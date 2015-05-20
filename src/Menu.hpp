#pragma once

#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>

class Menu: public Scene
{
public:
	// Command enum defines the relative order of elements
	enum Command {LEVEL, EDITOR, OPTIONS, CREDITS, EXIT};	// TODO move this to global?
	const std::map<Command, std::string> cmdMap;
	struct Entry
	{
		Entry(std::string name, const sf::Font& font, sf::Color color, unsigned int charSize, sf::Vector2f center, float angle, float radius, Command cmd):
			center(center),
			rotAngle(0),
			angle(angle),
			radius(radius),
			cmd(cmd)
		{
			text.setFont(font);
			text.setCharacterSize(charSize);
			text.setColor(color);
			text.setString(name);
		}
		void rotate(float angle)
		{
			rotAngle += angle;
		}
		void update(float percentage)
		{
			if (fabs(rotAngle) < 0.005f)
			{
				angle += rotAngle;
				rotAngle = 0.0f;
			}
			else
			{
				angle += percentage * rotAngle;
				rotAngle = (1.0f - percentage) * rotAngle;
			}
			
			sf::Vector2f textSize(text.getLocalBounds().width, text.getLocalBounds().height);
			sf::Vector2f dir(sin(angle), cos(angle));
			sf::Vector2f pos = center + dir * radius - textSize / 2.0f;
			text.setPosition(pos);
		}
		sf::Text text;
		const sf::Vector2f center;
		float rotAngle;
		float angle;
		const float radius;
		Command cmd;
	};
	
	Menu(Command initialCmd = LEVEL);
	Scene* processEvent(sf::Event event, sf::RenderWindow& window) override final;
	Scene* update(sf::Time deltaT, sf::RenderWindow& window) override final;
	void draw(sf::RenderTarget& target, bool focus) override final;
private:
	unsigned int nextPos(unsigned int pos, unsigned int size, bool clockWise);
	void rotate(bool clockWise);
	std::vector<Entry> _entries;
	unsigned int _currentEntry;
	std::vector<unsigned int> _levels;
	unsigned int _currentLevel;
};
