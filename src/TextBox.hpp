#pragma once

#include <string>
#include <vector>
#include "TextElement.hpp"
#include <SFML/Graphics.hpp>
class TextBox {
public:
	TextBox();
	void appendText(TextElement* item);
	void triggerText(std::string eventType);
	bool enabled();
	void update(sf::Time deltaT);
private:
	void pushText();
	std::vector<TextElement*> elements;
	std::string currentEvent;
	TextElement* currentElement;
	bool actionPressed;
	bool skipPressed;
};
