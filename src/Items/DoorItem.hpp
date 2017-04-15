/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
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
	void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	virtual void update(sf::Time deltaTime);
	void toggle();
};
