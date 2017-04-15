/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include <queue>
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

class TileMap;

class Player: public Entity
{
public:
	Player(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Vector2f& doggieSize);
	void move(sf::Time deltaTime, const sf::Vector2f& moveDir, const sf::Vector2f& sceneSize, TileMap* map);	// TODO override final?
	void update(sf::Time deltaTime);
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
private:
	float _animationStep;
	int _direction;
	sf::RectangleShape _shape;
	sf::RectangleShape _doggieShape;
	std::queue<sf::Vector2f> _positionQueue;
	std::queue<int> _directionQueue;
	
};

const static int PlayerAnimState[] = {0, 1, 1, 0, 2, 2};
const static int DoggieAnimState[] = {0, 2, 1, 3, 1, 4};
