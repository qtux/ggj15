/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include <SFML/Graphics.hpp>
class TileMap;

class Entity
{
private:
	void atomicMove(sf::Time deltaTime, const sf::Vector2f& moveDir, const sf::Vector2f& sceneSize, TileMap* map);
protected:
	float _velocity;		// TODO make int?
	sf::IntRect _collider;
public:
	Entity(float velocity, const sf::IntRect& collider);
	bool intersects(const sf::Vector2i& tilePosition, const sf::Vector2i& tileSize);
	bool intersects(const Entity& other, sf::IntRect& intersection);
	bool intersects(const Entity& other);
	void move(sf::Time deltaTime, const sf::Vector2f& moveDir, const sf::Vector2f& sceneSize, TileMap* map);
};
