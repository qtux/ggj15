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
	TextBox();
	void appendText(TextElement* item);
	void triggerText(std::string eventType);
	bool enabled();
	void pushText();
	void update();
	void draw(sf::RenderTarget &renderTarget);
private:
	std::vector<TextElement*> elements;
	std::string currentEvent;
	TextElement* currentElement;
	bool actionPressed;
	bool skipPressed;
};
