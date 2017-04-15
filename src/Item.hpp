/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include <SFML/Graphics.hpp>
class Level;

class Item
{
public:
	Item(sf::Sprite* sprite,int x,int y,int w,int h);
	virtual ~Item(){}
	virtual void update(sf::Time deltaTime);
	virtual void draw(sf::RenderTarget &renderTarget, sf::Shader *renderShader);
	virtual void applyEffect(Level& level) {}
	const sf::Vector2f& getPosition() const;
	void setPosition(float x, float y);
	sf::Sprite * mySprite;
	bool collectable;
	bool blocksPath;
private:
	float phaseOffset;
	sf::Clock waveClock;
	sf::Vector2f position;
};
