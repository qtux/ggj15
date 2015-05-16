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
	void move(sf::Time deltaTime, const sf::Vector2f& moveDir, const sf::Vector2f& sceneSize, TileMap* map);
};
