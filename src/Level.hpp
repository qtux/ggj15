/*
 * Level.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 * hack hier, ne doch nicht xD
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

class Level: public Scene {
public:
	Level(unsigned int levelNumber);
	const unsigned int levelNumber;
	unsigned int restarts;
	
	void reset();
	bool readyToLeave() const;
	void leave();
	Scene* processEvent(sf::Event event, sf::RenderWindow& window) override final;
	Scene* update(sf::Time deltaT, sf::RenderWindow& window) override final;
	void draw(sf::RenderTarget& target, bool focus) override final;
	void switchLargeTile(const sf::Vector2u& first, const sf::Vector2u& second);
	
	// move to private
	GUI* gui;
	TextBox* textBox;
private:
	sf::Uint32 createColorKey(sf::Color color);
	enum State {GAME, LEAVING, HIGHSCORE};
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
