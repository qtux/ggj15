/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

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
