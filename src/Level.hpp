/*
 * Level.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#pragma once

#include <SFML/System.hpp>
#include <vector>
class Player;
#include "GUI.hpp"
#include "Item.hpp"
#include "TextBox.hpp"
#include "Highscore.hpp"
#include "Scene.hpp"

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
	
	GameObject* getTile(int x, int y);
	void switchLargeTile(const sf::Vector2u& first, const sf::Vector2u& second);
	void reset();
	
	Scene* processEvent(sf::Event event, sf::RenderWindow& window) override final;
	Scene* update(sf::Time deltaT, sf::RenderWindow& window) override final;
	void draw(sf::RenderTarget& target, bool focus) override final;
	
	void leave();
	// hack hier, ne doch nicht xD
	Player* player;
	GUI* gui;
	TextBox* textBox;
	const std::vector<GameObject*> & getGameBoard() const;
	sf::Vector2i startPos;
	sf::Vector2i portalPos;
	std::vector<Item*> items;
	const unsigned int levelNumber;
//private:
	std::vector<GameObject*> gameBoard;
	std::vector<TileFlightData> tileAnimationPos;
	float tileAnimationTime;
	void updateTileAnimation(sf::Time deltaT);
	bool readyToLeave() const;
	void finishLevel();
	unsigned int restarts;
private:
	sf::Uint32 createColorKey(sf::Color color);
	enum State {GAME, LEAVING, HIGHSCORE};
	State _state;
	Highscore *highscore;
	sf::RectangleShape outline;
	sf::Shader _fragmentShader;
};
