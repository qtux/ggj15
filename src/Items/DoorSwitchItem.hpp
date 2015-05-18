#pragma once

#include "../Item.hpp"

class Level;

class DoorSwitchItem: public Item
{
private:
	bool toggled;
	sf::IntRect doorSwitchLeftTextureRect;
	sf::IntRect doorSwitchRightTextureRect;
public:
	DoorSwitchItem(sf::Sprite* sprite, bool toggled = false);
	void applyEffect(Level& level);
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	virtual void update(sf::Time deltaTime);
};
