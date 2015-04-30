/*
 * Level.hpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#pragma once

#include <SFML/System.hpp>
#include <vector>
#include "GameObject.hpp"
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
	Level(unsigned int number);
	
	GameObject* getTile(int x, int y);
	void switchLargeTile(int x1, int y1, int x2, int y2);
	void reset();
	
	Scene* processEvent(sf::Event event, sf::RenderWindow& window) override final;
	void update(sf::Time deltaT, sf::RenderWindow& window) override final;
	void draw(sf::RenderTarget& target, bool focus) override final;
	
	void leave();
	// hack hier, ne doch nicht xD
	GameObject * player;
	GUI* gui;
	TextBox* textBox;
	const std::vector<GameObject*> & getGameBoard() const;
	sf::Vector2i startPos;
	sf::Vector2i portalPos;
	std::vector<Item*> items;
	const unsigned int number;
//private:
	std::vector<GameObject*> gameBoard;
	std::vector<TileFlightData> tileAnimationPos;
	float tileAnimationTime;
	void updateTileAnimation(sf::Time deltaT);
	bool readyToLeave() const;
	void finishLevel();
private:
	sf::Uint32 createColorKey(sf::Color color);
	bool leaved;
	Highscore *highscore;
	bool fooexit;
	sf::RectangleShape outline;
	sf::Shader _fragmentShader;
};
