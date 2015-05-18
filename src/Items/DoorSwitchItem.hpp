#pragma once

#include "../Item.hpp"

class Level;

class DoorSwitchItem: public Item
{
private:
	bool toggled;
	sf::IntRect doorSwitchLeftTexture;
	sf::IntRect doorSwitchRightTexture;
public:
	DoorSwitchItem(sf::Sprite* sprite, bool toggled = false);
	virtual bool applyEffect(Level& level);
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	virtual void update(sf::Time deltaTime);
};
