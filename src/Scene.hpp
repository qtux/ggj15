/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include <SFML/Graphics.hpp>

class Scene
{
public:
	// requires size (width and height in world coordinates) of the rectangular space to be drawn
	Scene(const sf::Vector2f sceneSize):
		sceneSize(sceneSize),
		background(sf::RectangleShape(sceneSize)),
		overlay(sf::RectangleShape(sceneSize))
	{}
	virtual ~Scene() {}
	void resize(unsigned int newWidth, unsigned int newHeight, sf::RenderWindow& window)
	{
		// get ratio based on the original scene size
		float widthRatio = (float) newWidth / sceneSize.x;
		float heightRatio = (float) newHeight / sceneSize.y;
		float heightOffset = 0;
		float widthOffset = 0;
		float ratio = 1;
		
		// use the smaller ratio to update the window size (to keep the original ratio)
		if (heightRatio > widthRatio)
		{
			// black border up and down (undesirable)
			ratio = widthRatio;
			heightOffset += (newHeight - sceneSize.y * ratio) / 2.0f;
		}
		else
		{
			// black border left and right
			ratio = heightRatio;
			widthOffset += (newWidth - sceneSize.x * ratio) / 2.0f;
		}
		
		// determine position and view size
		sf::Vector2f viewPosition(-widthOffset, -heightOffset);
		viewPosition /= ratio;
		sf::Vector2f viewSize(newWidth, newHeight);
		viewSize /= ratio;
		// set view
		window.setView(sf::View(sf::FloatRect(viewPosition, viewSize)));
		// set background and overlay
		background.setPosition(viewPosition);
		background.setSize(viewSize);
		overlay.setPosition(viewPosition);
		overlay.setSize(viewSize);
	}
	// pure virtual member functions (i.e. abstract member functions)
	virtual Scene* processEvent(sf::Event event, sf::RenderWindow& window) = 0;
	virtual Scene* update(sf::Time deltaT, sf::RenderWindow& window) = 0;
	virtual void draw(sf::RenderTarget& target, bool focus) = 0;
protected:
	const sf::Vector2f sceneSize;
	sf::RectangleShape background;
	sf::RectangleShape overlay;
};
