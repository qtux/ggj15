#include "Menu.hpp"
#include "Editor.hpp"
#include "Level.hpp"
#include "Credits.hpp"

Menu::Menu(Menu::Command initialCmd):
	Scene({800, 600}),
	cmdMap({{EDITOR, "Editor"}, {LEVEL, "Level"}, {EXIT, "Exit"}, {OPTIONS, "Options"}, {CREDITS, "Credits"}}),
	_currentEntry(0),
	_levels({0,1,2,3,4,5,6,7,8,9}),
	_currentLevel(0)
{
	// create entries
	_font.loadFromFile("./fonts/LiberationSerif-Regular.ttf");
	sf::Vector2f center(sceneSize / 2.0f);
	float radius = sceneSize.x / 4.0f;
	float angle = M_PI;
	for (auto& kv: cmdMap)
	{
		_entries.push_back(Entry(kv.second, _font, sf::Color(0x00, 0x00, 0x00), 32, center, angle, radius, kv.first));
		angle += 2.0f * M_PI / cmdMap.size();
	}
	// set first entry on top
	for (auto i = 0; i < _entries.size(); ++i)
	{
		if (_entries[i].cmd == initialCmd)
		{
			break;
		}
		rotate(true);
	}
	//background.setTexture(gb::textureManager.getTexture("./img/background.png", true));
	overlay.setFillColor(sf::Color(0, 0, 0, 200));
}

unsigned int Menu::nextPos(unsigned int pos, unsigned int size, bool clockWise)
{
	auto forward = (pos < size - 1) ? pos + 1 : 0;
	auto backward = (pos > 0) ? pos - 1 : size - 1;
	return clockWise ? forward : backward;
}

void Menu::rotate(bool clockWise)
{
	float angle = 2.0f * M_PI / (float) _entries.size();
	angle *= clockWise ? -1.0f : 1.0f;
	_currentEntry = nextPos(_currentEntry, _entries.size(), clockWise);
	for (auto& entry: _entries)
	{
		entry.rotate(angle);
	}
}

Scene* Menu::processEvent(sf::Event event, sf::RenderWindow& window)
{
	// preprocessing key input (to enhance code readability)
	sf::Keyboard::Key keyPressed = sf::Keyboard::Unknown;
	if (event.type == sf::Event::KeyPressed)
	{
		keyPressed = event.key.code;
	}
	
	bool levelSelected = (_entries[_currentEntry].cmd == LEVEL);
	
	switch(keyPressed)
	{
		case sf::Keyboard::Escape:
			window.close();
			break;
		case sf::Keyboard::Right:
			rotate(true);
			break;
		case sf::Keyboard::Left:
			rotate(false);
			break;
		case sf::Keyboard::Up:
			if (levelSelected)
			{
				_currentLevel = nextPos(_currentLevel, _levels.size(), true);
			}
			break;
		case sf::Keyboard::Down:
			if (levelSelected)
			{
				_currentLevel = nextPos(_currentLevel, _levels.size(), false);
			}
			break;
		case sf::Keyboard::Return:
			// load a scene dependend on the current command
			switch(_entries[_currentEntry].cmd)
			{
				case EDITOR:
					return new Editor();
				case LEVEL:
					return new Level(_currentLevel);
					break;
				case EXIT:
					window.close();
					break;
				case OPTIONS:
					/*TODO*/
					break;
				case CREDITS:
					return new Credits();
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return this;
}
Scene* Menu::update(sf::Time deltaT, sf::RenderWindow& window)
{
	for (auto& entry: _entries)
	{
		entry.update(deltaT.asSeconds() * 10.0f);
	}
	return this;
}

void Menu::draw(sf::RenderTarget& target, bool focus)
{
	target.draw(background);
	for (auto& entry: _entries)
	{
		target.draw(entry.text);
	}
	if (!focus)
	{
		target.draw(overlay);
	}
}
