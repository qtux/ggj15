#pragma once

#include <SFML/Graphics.hpp>

class Scene
{
public:
	// requires width and height (in world coordinates) of the rectangular space to be drawn
	Scene(unsigned int width, unsigned int height):
		width(width),
		height(height),
		background(sf::RectangleShape(sf::Vector2f(width, height)))
	{}
	virtual ~Scene(){}
	void resize(int nextWidth, int nextHeight, sf::RenderWindow& window)
	{
		// get ratio based on the original size
		float widthRatio = (float) nextWidth / width;
		float heightRatio = (float) nextHeight / height;
		float heightOffset = 0;
		float widthOffset = 0;
		float ratio = 1;
		
		// use the smaller ratio to update the window size (to keep the original ratio)
		if (heightRatio > widthRatio)
		{
			// black border up and down (undesirable)
			ratio = widthRatio;
			heightOffset += (nextHeight - height * ratio) / 2.0f;
		}
		else
		{
			// black border left and right
			ratio = heightRatio;
			widthOffset += (nextWidth - width * ratio) / 2.0f;
		}
		
		// determine position and size of view and background
		sf::Vector2f position(-widthOffset, -heightOffset);
		position /= ratio;
		sf::Vector2f size(nextWidth, nextHeight);
		size /= ratio;
		// set view and background
		window.setView(sf::View(sf::FloatRect(position, size)));
		background.setPosition(position);
		background.setSize(size);
	}
	sf::Vector2f getMouseWorldPos(sf::RenderWindow& window)
	{
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		return window.mapPixelToCoords(pixelPos);
	}
	virtual Scene* processEvent(sf::Event event, sf::RenderWindow& window) = 0;
	virtual void update(sf::Time deltaT, sf::RenderWindow& window) = 0;
	virtual void draw(sf::RenderTarget& target) = 0;
	const unsigned int width;
	const unsigned int height;
	sf::RectangleShape background;
	sf::Shader shader;
};
