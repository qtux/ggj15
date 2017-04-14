#include "TextBox.hpp"
#include "global.hpp"

TextBox::TextBox():
	currentElement(nullptr)
{
	int charSize = gb::pixelSizeY;
	sf::Vector2f textPos(32.0f, 32.0f);
	_speech.setFont(gb::ressourceManager.getFont(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf"));
	_speech.setCharacterSize(charSize);
	_speech.setPosition(textPos);
	_textRect.setOutlineColor(sf::Color::White);
	_textRect.setOutlineThickness(2);
	_textRect.setPosition(textPos.x - 5, textPos.y - 5);
	_textRect.setSize(sf::Vector2f(gb::gridWidth - 2* textPos.x + 10, 2 * charSize + 30));
	_textRect.setFillColor(sf::Color(0, 0, 250, 50));
}

void TextBox::appendText(TextElement* item)
{
	elements.push_back(item);
}

void TextBox::triggerText(std::string eventType)
{
	bool exists = false;
	for (auto& text: elements)
	{
		if (text->eventType == eventType)
		{
			exists = true;
			break;
		}
	}
	if (exists)
	{
		currentEvent = eventType;
		pushText();
	}
}

void TextBox::pushText()
{
	for(std::vector<TextElement*>::iterator itIt = elements.begin() ; itIt != elements.end() ;itIt++ ) {
		if ((*itIt)->eventType == currentEvent)
		{
			currentElement = new TextElement();
			currentElement->eventType = (*itIt)->eventType;
			currentElement->bold = (*itIt)->bold;
			currentElement->r = (*itIt)->r;
			currentElement->g = (*itIt)->g;
			currentElement->b = (*itIt)->b;
			currentElement->text = (*itIt)->text;
			itIt = elements.erase(itIt);
			return;
		} 
	}
	currentElement = nullptr;
}

bool TextBox::enabled()
{
	return currentElement != nullptr;
}

void TextBox::skip()
{
	for(std::vector<TextElement*>::iterator itIt = elements.begin() ; itIt != elements.end() ;itIt++ )
	{
		if ((*itIt)->eventType == currentEvent)
		{
			itIt = elements.erase(itIt);
		} 
	}
	currentElement = nullptr;
}

void TextBox::draw(sf::RenderTarget &renderTarget)
{
	if (currentElement != nullptr)
	{
		// draw background
		renderTarget.draw(_textRect);
		
		// draw text
		if (currentElement->bold){
			_speech.setStyle(sf::Text::Bold);
		}
		else
		{
			_speech.setStyle(sf::Text::Regular);
		}
		_speech.setFillColor(sf::Color(currentElement->r, currentElement->g, currentElement->b));
		_speech.setString(currentElement->text);
		renderTarget.draw(_speech);
	}
}
