/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class TextBox
{
public:
	struct TextElement
	{
		std::string eventType;
		bool bold;
		int r;
		int g;
		int b;
		std::string text;
	};
	TextBox(const sf::Vector2f& sceneSize);
	void appendText(TextElement* item);
	void triggerText(std::string eventType);
	bool enabled();
	void pushText();
	void skip();
	void draw(sf::RenderTarget &renderTarget);
private:
	std::vector<TextElement*> elements;
	std::string currentEvent;
	TextElement* currentElement;
	sf::Text _speech;
	sf::RectangleShape _textRect;
};
