/*
 * Level.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 * hack hier, ne doch nicht xD
 */

#pragma once

#include <SFML/System.hpp>
#include <vector>
#include "Scene.hpp"
class Highscore;
#include "TextBox.hpp"
class Item;
#include "GUI.hpp"
class Player;
#include "GameObject.hpp"
#include "NPC.hpp"

struct TileFlightData
{
	GameObject *tile;
	sf::Vector2f currentPos;
	sf::Vector2f targetPos;
	sf::Vector2f momentum;
	sf::Vector2f startPos;
	float scale;
};

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
	
	// to be removed:
	GameObject* getTile(int x, int y);
	const std::vector<GameObject*> & getGameBoard() const;
	sf::Vector2i startPos;
	sf::Vector2i portalPos;
	std::vector<NPC*> npcs;
	const unsigned int number;

	std::vector<GameObject*> gameBoard;
	std::vector<TileFlightData> tileAnimationPos;
	float tileAnimationTime;
	void updateTileAnimation(sf::Time deltaT);
	
	// move to private
	GUI* gui;
	TextBox* textBox;
	std::vector<Item*> items;
private:
	sf::Uint32 createColorKey(sf::Color color);
	enum State {GAME, LEAVING, HIGHSCORE};
	State _state;
	Player* player;
	Highscore *highscore;
	sf::Shader _fragmentShader;
	sf::RectangleShape _outline;
};
