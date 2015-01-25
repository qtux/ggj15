#include "TextBox.hpp"
#include "globals.hpp"

TextBox::TextBox(){
	currentElement = 0;
	skipPressed = false;
	actionPressed = false;
}

void TextBox::appendText(TextElement* item)
{
	elements.push_back(item);
}

void TextBox::triggerText(std::string eventType)
{
	bool exits = false;
	for(std::vector<TextElement*>::iterator itIt = elements.begin() ; itIt != elements.end() ;itIt++ ) {
		if ((*itIt)->eventType == eventType)
		{
			exits = true;
			break;
		}
	}
	if (exits){
		currentEvent = eventType;
		pushText();
	}
}

void TextBox::pushText()
{
	for(std::vector<TextElement*>::iterator itIt = elements.begin() ; itIt != elements.end() ;itIt++ ) {
		if ((*itIt)->eventType == currentEvent)
		{
			currentElement = (*itIt);
			itIt = elements.erase(itIt);
			return;
		} 
	}
	currentElement = 0;
}

bool TextBox::enabled()
{
	return currentElement != 0;
}
void TextBox::update(sf::Time deltaT)
{
	if (!input[4] && actionPressed)
	{
		pushText();
	}
	if (!input[5] && skipPressed)
	{
		for(std::vector<TextElement*>::iterator itIt = elements.begin() ; itIt != elements.end() ;itIt++ ) {
			if ((*itIt)->eventType == currentEvent)
			{
				itIt = elements.erase(itIt);
			} 
		}
		currentElement = 0;
	}
	actionPressed = input[4];
	skipPressed = input[5];
	if (currentElement != 0)
	{
		sf::Font font;
		font.loadFromFile(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf");
		// Text TEST
		sf::Vector2f textPos(32.0f, 32.0f);
		int charSize = 10;
	
		sf::Text speech;
		speech.setFont(font);
	
		speech.setColor(sf::Color(currentElement->r, currentElement->g, currentElement->b));
		speech.setCharacterSize(charSize);
		speech.setPosition(textPos);
	
		sf::RectangleShape textRect;
		textRect.setOutlineColor(sf::Color::White);
		textRect.setOutlineThickness(2);
		textRect.setPosition(textPos.x - 5, textPos.y - 5);
		textRect.setSize(sf::Vector2f(gridWidth - 2* textPos.x + 10, 2 * charSize + 30));
		textRect.setFillColor(sf::Color(0, 0, 250, 50));
		window.draw(textRect);
	
		// zu Anfang:
		if (currentElement->bold){
			speech.setStyle(sf::Text::Bold);
		}
		else
		{
			speech.setStyle(sf::Text::Regular);
		}
		speech.setString(currentElement->text);
		window.draw(speech);
	}
}