#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
private:
	void atomicMove();
protected:
	float _velocity;		// TODO make int?
	sf::IntRect _collider;
public:
	Entity(float velocity, const sf::IntRect& collider);
	void move();
};
