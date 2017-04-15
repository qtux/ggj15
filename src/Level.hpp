/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include <SFML/System.hpp>
#include <map>
#include "Scene.hpp"
class Highscore;
#include "TextBox.hpp"
class Item;
#include "GUI.hpp"
class Player;
class TileMap;
//#include "NPC.hpp"

class Level: public Scene {
public:
	Level(unsigned int levelNumber);
	const unsigned int levelNumber;
	unsigned int restarts;
	
	void reset();
	bool readyToLeave() const;
	void leave();
	void toggleDoors();
	Scene* processEvent(sf::Event event, sf::RenderWindow& window) override final;
	Scene* update(sf::Time deltaT, sf::RenderWindow& window) override final;
	void draw(sf::RenderTarget& target, bool focus) override final;
	void switchLargeTile(const sf::Vector2u& first, const sf::Vector2u& second);
	
	// move to private
	GUI* gui;
	TextBox* textBox;
	//std::vector<NPC*> npcs;
private:
	bool parseLevel(std::string fileName);
	enum State {GAME, LEAVING, HIGHSCORE, ABORT};
	State _state;
	struct Key
	{
		Key() = delete;
		Key(unsigned int x, unsigned int y):
			x(x), y(y)
		{}
		unsigned int x;
		unsigned int y;
		bool operator<(const Key& rhs) const
		{
			// compare x then compare y
			return std::tie(x, y) < std::tie(rhs.x, rhs.y);
		}
	};
	Player* player;
	Highscore *highscore;
	TileMap* map;
	sf::Shader _fragmentShader;
	sf::RectangleShape _outline;
	std::map<Key, Item*> items;
};
