#include "TextBox.hpp"
#include "global.hpp"

TextBox::TextBox():
	//currentEvent(""),
	currentElement(nullptr),
	actionPressed(false),
	skipPressed(false)
{}

void TextBox::appendText(TextElement* item)
{
	elements.push_back(item);
}

void TextBox::triggerText(std::string eventType)
{
	bool exists = false;
	for(std::vector<TextElement*>::iterator itIt = elements.begin() ; itIt != elements.end() ;itIt++ ) {
		if ((*itIt)->eventType == eventType)
		{
			exists = true;
			break;
		}
	}
	if (exists){
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

// TODO rename into skip and remove input?
void TextBox::update()
{
	if (!gb::input[4] && actionPressed)
	{
		pushText();
	}
	if (!gb::input[5] && skipPressed)
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
	actionPressed = gb::input[4];
	skipPressed = gb::input[5];
}

void TextBox::draw(sf::RenderTarget &renderTarget)
{
	if (currentElement != nullptr)
	{
		sf::Font font;
		font.loadFromFile(std::string(PATH) + "fonts/LiberationSerif-Regular.ttf");
		// Text TEST
		sf::Vector2f textPos(32.0f, 32.0f);
		int charSize = 15;
	
		sf::Text speech;
		speech.setFont(font);
	
		speech.setColor(sf::Color(currentElement->r, currentElement->g, currentElement->b));
		speech.setCharacterSize(charSize);
		speech.setPosition(textPos);
	
		sf::RectangleShape textRect;
		textRect.setOutlineColor(sf::Color::White);
		textRect.setOutlineThickness(2);
		textRect.setPosition(textPos.x - 5, textPos.y - 5);
		textRect.setSize(sf::Vector2f(gb::gridWidth - 2* textPos.x + 10, 2 * charSize + 30));
		textRect.setFillColor(sf::Color(0, 0, 250, 50));
		renderTarget.draw(textRect);
	
		// zu Anfang:
		if (currentElement->bold){
			speech.setStyle(sf::Text::Bold);
		}
		else
		{
			speech.setStyle(sf::Text::Regular);
		}
		speech.setString(currentElement->text);
		renderTarget.draw(speech);
	}
}
