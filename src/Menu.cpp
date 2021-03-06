/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "Menu.hpp"
#include "Editor.hpp"
#include "Level.hpp"
#include "Credits.hpp"
#include <fstream>
#include <sstream>

Menu::Menu(Menu::Command initialCmd, unsigned int currentLevel):
	Scene(gb::sceneSize),
	cmdMap({{EDITOR, "Editor"}, {LEVEL, "Level"}, {EXIT, "Exit"}, {OPTIONS, "Options"}, {CREDITS, "Credits"}}),
	_currentEntry(0),
	_currentLevel(currentLevel),
	_levelMap(TileMap(gb::tileSize, gb::gridSize, std::string(PATH) + "levels/level" + std::to_string(currentLevel)))
{
	// read level number from file
	std::ifstream indexFile("levels/index.txt");
	if (indexFile.is_open())
	{
		unsigned int level;
		while (indexFile >> level)
		{
			_levels.push_back(level);
		}
	}
	std::sort(_levels.begin(), _levels.end());
	std::unique(_levels.begin(), _levels.end());
	// TODO use a set instead of a vector to keep track of all levels
	
	// create entries
	sf::Vector2f center(sceneSize / 2.0f);
	float radius = sceneSize.x / 4.0f;
	float angle = M_PI;
	for (auto& kv: cmdMap)
	{
		_entries.push_back(Entry(kv.second, gb::ressourceManager.getFont(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf"), sf::Color(0xcc, 0x22, 0x22), 42, center, angle, radius, kv.first));
		angle += 2.0f * M_PI / cmdMap.size();
	}
	// set first entry on top
	for (auto i = 0; i < _entries.size(); ++i)
	{
		if (_entries[i].cmd == initialCmd)
		{
			break;
		}
		rotate(true);
	}
	overlay.setFillColor(sf::Color(0, 0, 0, 200));
	_entries[LEVEL].appendText(" "  + std::to_string(_currentLevel));
}

unsigned int Menu::nextPos(unsigned int pos, unsigned int size, bool clockWise)
{
	auto forward = (pos < size - 1) ? pos + 1 : 0;
	auto backward = (pos > 0) ? pos - 1 : size - 1;
	return clockWise ? forward : backward;
}

void Menu::rotate(bool clockWise)
{
	float angle = 2.0f * M_PI / (float) _entries.size();
	angle *= clockWise ? -1.0f : 1.0f;
	_currentEntry = nextPos(_currentEntry, _entries.size(), clockWise);
	for (auto& entry: _entries)
	{
		entry.rotate(angle);
	}
}

Scene* Menu::processEvent(sf::Event event, sf::RenderWindow& window)
{
	// preprocessing key input (to enhance code readability)
	sf::Keyboard::Key keyPressed = sf::Keyboard::Unknown;
	if (event.type == sf::Event::KeyPressed)
	{
		keyPressed = event.key.code;
	}
	
	bool levelSelected = (_entries[_currentEntry].cmd == LEVEL);
	
	switch(keyPressed)
	{
		case sf::Keyboard::Escape:
			window.close();
			break;
		case sf::Keyboard::Right:
			rotate(true);
			break;
		case sf::Keyboard::Left:
			rotate(false);
			break;
		case sf::Keyboard::Up:
			if (levelSelected)
			{
				_currentLevel = _levels[nextPos(_currentLevel, _levels.size(), true)];
				_entries[LEVEL].appendText(" "  + std::to_string(_currentLevel));
				_levelMap = TileMap(gb::tileSize, gb::gridSize, std::string(PATH) + "levels/level" + std::to_string(_currentLevel));
			}
			break;
		case sf::Keyboard::Down:
			if (levelSelected)
			{
				_currentLevel = _levels[nextPos(_currentLevel, _levels.size(), false)];
				_entries[LEVEL].appendText(" "  + std::to_string(_currentLevel));
				_levelMap = TileMap(gb::tileSize, gb::gridSize, std::string(PATH) + "levels/level" + std::to_string(_currentLevel));
			}
			break;
		case sf::Keyboard::Return:
			// load a scene dependend on the current command
			switch(_entries[_currentEntry].cmd)
			{
				case EDITOR:
					return new Editor();
				case LEVEL:
					return new Level(_currentLevel);
					break;
				case EXIT:
					window.close();
					break;
				case OPTIONS:
					/*TODO*/
					break;
				case CREDITS:
					return new Credits();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return this;
}
Scene* Menu::update(sf::Time deltaT, sf::RenderWindow& window)
{
	for (auto& entry: _entries)
	{
		entry.update(deltaT.asSeconds() * 10.0f);
	}
	return this;
}

void Menu::draw(sf::RenderTarget& target, bool focus)
{
	_levelMap.draw(target);
	for (auto& entry: _entries)
	{
		target.draw(entry.text);
	}
	if (!focus)
	{
		target.draw(overlay);
	}
}
