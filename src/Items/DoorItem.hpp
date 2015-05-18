/*
 * DoorItem.h
 *
 *  Created on: 28.02.2015
 *      Author: johannes
 */

#pragma once

#include "../Item.hpp"

class Level;

class DoorItem: public Item {
private:
	bool wasClosed;
	bool vertical;
	sf::IntRect openDoorTextureRect;
	sf::IntRect closedDoorTextureRect;
public:
	DoorItem(sf::Sprite* sprite, bool vertical_, bool closed = true);
	virtual bool applyEffect(Level& level);
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	virtual void update(sf::Time deltaTime);
	void toggle();
};
