#pragma once

#include "Scene.hpp"

class Credits: public Scene
{
public:
	Credits();
	Scene* processEvent(sf::Event event, sf::RenderWindow& window) override final;
	Scene* update(sf::Time deltaT, sf::RenderWindow& window) override final;
	void draw(sf::RenderTarget& target, bool focus) override final;
private:
	sf::Text speech;
};
