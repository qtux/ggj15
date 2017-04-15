/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include "TileMap.hpp"

class Menu: public Scene
{
public:
	// Command enum defines the relative order of elements
	enum Command {LEVEL, EDITOR, OPTIONS, CREDITS, EXIT};	// TODO move this to global?
	const std::map<Command, std::string> cmdMap;
	struct Entry
	{
		Entry(std::string name, const sf::Font& font, sf::Color color, unsigned int charSize, sf::Vector2f center, float angle, float radius, Command cmd):
			name(name),
			center(center),
			rotAngle(0),
			angle(angle),
			radius(radius),
			cmd(cmd)
		{
			text.setFont(font);
			text.setStyle(sf::Text::Bold);
			text.setCharacterSize(charSize);
			text.setFillColor(color);
			text.setOutlineColor(sf::Color(0x00, 0x00, 0x00));
			text.setOutlineThickness(2.0f);
			text.setString(name);
		}
		void rotate(float angle)
		{
			rotAngle += angle;
		}
		void update(float percentage)
		{
			if (std::fabs(rotAngle) < 0.005f)
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
			sf::Vector2f dir(std::sin(angle), std::cos(angle));
			sf::Vector2f pos = center + dir * radius - textSize / 2.0f;
			text.setPosition(pos);
		}
		void appendText(std::string appendix)
		{
			text.setString(name + appendix);
		}
		sf::Text text;
		const std::string name;
		const sf::Vector2f center;
		float rotAngle;
		float angle;
		const float radius;
		Command cmd;
	};
	
	Menu(Command initialCmd = LEVEL, unsigned int currentLevel = 0);
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
	const sf::Vector2u _gridSize, _tileSize;
	TileMap _levelMap;
};
