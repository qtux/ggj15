#include "Editor.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "global.hpp"
#include <iostream>
#include "Menu.hpp"

Editor::Editor():
	Scene(mapWidth + 2 * lateralOffset, mapHeight + lateralOffset)
{
	// fill color choices array
	_tileTypeBinding[0] = sf::Color::White;//0xffffffff;
	_tileTypeBinding[1] = sf::Color(17, 148, 27);//0x11941bff;
	_tileTypeBinding[2] = sf::Color(155, 109, 39);//0x9b6d27ff;
	_tileTypeBinding[3] = sf::Color(150, 152, 150);//0x969896ff;
	_tileTypeBinding[4] = sf::Color(95, 95, 95);//0x5f5f5fff;
	_tileTypeBinding[5] = sf::Color(0, 1, 0);//0x000100ff;
	_tileTypeBinding[6] = sf::Color(0, 0, 171);//0x0000abff;
	_tileTypeBinding[7] = sf::Color(00, 62, 4);//0x003E04ff;
	// create tiles
	for (int x = 0; x < numTilesX; ++x)
	{
		for (int y = 0; y < numTilesY; ++y)
		{
			sf::RectangleShape* tile = new sf::RectangleShape();
			tile->setFillColor(sf::Color::White);
			tile->setOutlineColor(sf::Color::Blue);
			tile->setOutlineThickness(1.0f);
			tile->setPosition(lateralOffset + x * tileOffset + 1.0f, y * tileOffset + 1.0f);
			tile->setSize(sf::Vector2f(tileSize, tileSize));
			tiles[x][y] = tile;
		}
	}
	// create color/tile choices
	for (int y = 0; y < tileChoices.size(); ++y)
	{
		sf::RectangleShape* tileChoice = new sf::RectangleShape();
		tileChoice->setFillColor(_tileTypeBinding[y]);
		tileChoice->setOutlineColor(sf::Color::Black);
		tileChoice->setOutlineThickness(1.0f);
		tileChoice->setPosition(0.5 * tileOffset, y * tileOffset + 1.0f);
		tileChoice->setSize(sf::Vector2f(tileSize, tileSize));
		tileChoices[y] = tileChoice;
	}
	tileChoices[0]->setOutlineColor(sf::Color::Red);
	// create item choices
	for (int y = 0; y < itemChoices.size(); ++y)
	{
		sf::RectangleShape* itemChoice = new sf::RectangleShape();
		itemChoice->setFillColor(_tileTypeBinding[y] + sf::Color::Yellow);
		itemChoice->setOutlineColor(sf::Color::Black);
		itemChoice->setOutlineThickness(1.0f);
		itemChoice->setPosition(lateralOffset + mapWidth + tileOffset, y * tileOffset + 1.0f);
		itemChoice->setSize(sf::Vector2f(tileSize, tileSize));
		itemChoices[y] = itemChoice;
	}
	
	activeColorIndex = 0;
	activeItemIndex = -1;
	mousePressed = false;
	shiftActive = false;
	triggerMarkingActive = false;
	
	// items
	// 1. start (einmalig, nicht mehr auswaehlbar wenn schon gesetzt oder wird versetzt)
	// 2. portal (goal)
	// 3. trigger
	// 4. coin
	// 5. clock
	// 6. key
	// more
	// deco dialog with one item shown
}

Editor::~Editor()
{
	for (int x = 0; x < numTilesX; ++x)
	{
		for (int y = 0; y < numTilesY; ++y)
		{
			delete(tiles[x][y]);
		}
	}
	for (int y = 0; y < tileChoices.size(); ++y)
	{
		delete(tileChoices[y]);
	}
	for (int y = 0; y < itemChoices.size(); ++y)
	{
		delete(itemChoices[y]);
	}
}

// check recursively whether xPos is between x1 and x2 (per divide-and-conquer)
int Editor::isInX(const int xPos, int x1, int x2)
{
	if (x2 - x1 == 1)
	{
		return x1;
	}
	
	int x2Half = x1 + (x2 - x1) / 2;
	
	if (xPos >= (x1 * tileOffset) && xPos < (x2Half * tileOffset)) {
		return isInX(xPos, x1, x2Half);
	}
	if (xPos >= (x2Half * tileOffset) && xPos < (x2 * tileOffset)) {
		return isInX(xPos, x2Half, x2);
	}
	return -1;
}

// check recursively whether yPos is between y1 and y2 (per divide-and-conquer)
int Editor::isInY(const int yPos, int y1, int y2)
{
	if (y2 - y1 == 1)
	{
		return y1;
	}
	
	int y2Half = y1 + (y2 - y1) / 2;
	
	if (yPos >= (y1 * tileOffset) && yPos < (y2Half * tileOffset)) {
		return isInY(yPos, y1, y2Half);
	}
	if (yPos >= (y2Half * tileOffset) && yPos < (y2 * tileOffset)) {
		return isInY(yPos, y2Half, y2);
	}
	return -1;
}

// check if xPos and yPos are somewhere inside the bounderies of (x1,y1) and (x2,y2)
sf::Vector2i Editor::isIn(const int xPos, const int yPos, int x1, int y1, int x2, int y2)
{
	return sf::Vector2i(isInX(xPos, x1, x2), isInY(yPos, y1, y2));
}

// TODO: idea: change pen size
Scene* Editor::processEvent(sf::Event event, sf::RenderWindow& window)
{
	static sf::Vector2i markStart;
		
	// check ESC
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		return new Menu(Menu::Command::EDITOR);
	}
	
	// check for left mouseclick
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		mousePressed = true;
		sf::Vector2f mousePos = getMouseWorldPos(window);
		
		// if not in trigger marking mode where everything else is disabled
		if (!triggerMarkingActive)
		{
			// if item is chosen (x pos lateralOffset + mapWidth + tileOffset <- TODO probably a variable for this?)
			if (mousePos.x >= lateralOffset + mapWidth + tileOffset && mousePos.x <= lateralOffset + mapWidth + 2 * tileOffset && mousePos.y <= itemChoices.size() * tileOffset)
			{
				// unactivate tile type, activate item
				if (activeColorIndex != -1)
				{
					tileChoices[activeColorIndex]->setOutlineColor(sf::Color::Black);
				}
				if (activeItemIndex != -1)
				{
					itemChoices[activeItemIndex]->setOutlineColor(sf::Color::Black);
				}
				activeColorIndex = -1;
				activeItemIndex = mousePos.y / tileOffset;
				itemChoices[activeItemIndex]->setOutlineColor(sf::Color::Red);
			}
			
			// if tile type/color is chosen
			if (mousePos.x >= 0.5 * tileOffset && mousePos.x <= 1.5 * tileOffset && mousePos.y <= tileChoices.size() * tileOffset)
			{
				// unactivate item, activate tile type
				if (activeColorIndex != -1)
				{
					tileChoices[activeColorIndex]->setOutlineColor(sf::Color::Black);
				}
				if (activeItemIndex != -1)
				{
					itemChoices[activeItemIndex]->setOutlineColor(sf::Color::Black);
				}
				activeItemIndex = -1;
				activeColorIndex = mousePos.y / tileOffset;
				tileChoices[activeColorIndex]->setOutlineColor(sf::Color::Red);
			}
		}
		
		// if mouse in gameboard
		if (mousePos.x >= lateralOffset && mousePos.y >= 0 && mousePos.x <= lateralOffset + mapWidth && mousePos.y <= mapHeight)
		{
			// if color is active which is also when not in trigger marking mode (because switching to color is disabled)
			if (activeColorIndex != -1)
			{
				// if marking is drawn
				if (shiftActive)
				{
					// mark chosen tiles
					if (markStart.x != -1 && markStart.y != -1)
					{
						// find left upper corner and right lower corner of marking
						int x1 = std::min(markStart.x, (int) (mousePos.x - lateralOffset) / tileOffset);
						int x2 = std::max(markStart.x, (int) (mousePos.x - lateralOffset) / tileOffset);
						int y1 = std::min(markStart.y, (int) mousePos.y / tileOffset);
						int y2 = std::max(markStart.y, (int) mousePos.y / tileOffset);
						// color marking
						for (int x = x1; x <= x2; ++x)
						{
							for (int y = y1; y <= y2; ++y)
							{
								tiles[x][y]->setFillColor(_tileTypeBinding[activeColorIndex]);
							}
						}
					}
					// mark origin tile of marking
					if (markStart.x == -1 && markStart.y == -1)
					{
						markStart.x = (mousePos.x - lateralOffset) / tileOffset;
						markStart.y = mousePos.y / tileOffset;
						tiles[markStart.x][markStart.y]->setOutlineThickness(2.0f);
						tiles[markStart.x][markStart.y]->setOutlineColor(sf::Color::Red);
					}
				}
			}
			
			// if item is active
			if (activeItemIndex != -1)
			{
				static int marks;
				// if trigger marking is not active where everything else is disabled
				if (!triggerMarkingActive)
				{
					// set item
					tiles[(mousePos.x - lateralOffset) / tileOffset][mousePos.y / tileOffset]->setFillColor(_tileTypeBinding[activeItemIndex] + sf::Color::Yellow);
					if (activeItemIndex == 2) // trigger item
					{
						// force marking of quadrants
						triggerMarkingActive = true;
						// number of markings
						marks = 0;
					}
				}
				// if trigger marking is active, two correct markings must be drawn
				else
				{
					// TODO save these somewhere for this trigger
					int xPos = (mousePos.x - lateralOffset) / tileOffset;
					int yPos = mousePos.y / tileOffset;
					++marks;
					if (marks == 2)
					{
						triggerMarkingActive = false;
						if (xPos >= 0 && yPos >= 0 && xPos + quadrantSize <= numTilesX && yPos + quadrantSize <= numTilesY) {
							for (int x = xPos; x < xPos + quadrantSize; ++x)
							{
								for (int y = yPos; y < yPos + quadrantSize; ++y)
								{
									tiles[x][y]->setOutlineColor(sf::Color::Blue);
								}
							}
						}
					}
				}
			}
		}
	}
		
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{
		mousePressed = false;
	}
	
	// check for mousemove
	if (event.type == sf::Event::MouseMoved)
	{
		sf::Vector2f mousePos = getMouseWorldPos(window);
		// show marking
		if (triggerMarkingActive)
		{
			static int xPos;
			static int yPos;
			// delete old marking
			if (xPos >= 0 && yPos >= 0 && xPos + quadrantSize <= numTilesX && yPos + quadrantSize <= numTilesY) {
				for (int x = xPos; x < xPos + quadrantSize; ++x)
				{
					for (int y = yPos; y < yPos + quadrantSize; ++y)
					{
						tiles[x][y]->setOutlineColor(sf::Color::Blue);
					}
				}
			}
			// draw new marking
			xPos = (mousePos.x - lateralOffset) / tileOffset;
			yPos = mousePos.y / tileOffset;
			if (xPos >= 0 && yPos >= 0 && xPos + quadrantSize <= numTilesX && yPos + quadrantSize <= numTilesY) {
				for (int x = xPos; x < xPos + quadrantSize; ++x)
				{
					for (int y = yPos; y < yPos + quadrantSize; ++y)
					{
						tiles[x][y]->setOutlineColor(sf::Color::Red);
					}
				}
			}
		}
	}
	
	// mark on shift click
	if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift))
	{
		shiftActive = true;
		markStart.x = -1;
		markStart.y = -1;
	}
	
	// stop marking mode on shift release
	if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift))
	{
		shiftActive = false;
		// cleanup marked tile
		if (markStart.x != -1 && markStart.y != -1)
		{
			tiles[markStart.x][markStart.y]->setOutlineThickness(1.0f);
			tiles[markStart.x][markStart.y]->setOutlineColor(sf::Color::Blue);
		}
		markStart.x = -1;
		markStart.y = -1;
	}
	return this;
}

void Editor::update(sf::Time deltaT, sf::RenderWindow& window)
{
	// if mouse pressed to draw and shift is not pressed
	if (mousePressed && !shiftActive && activeColorIndex != -1)
	{
		sf::Vector2f mousePos = getMouseWorldPos(window);
		// if mouse in gameboard
		if (mousePos.x >= lateralOffset && mousePos.y >= 0 && mousePos.x <= lateralOffset + mapWidth && mousePos.y <= mapHeight)
		{
			/*
			// recursion
			sf::Vector2i xyPos = isIn(mousePos.x - lateralOffset, mousePos.y, 0, 0, numTilesX, numTilesY);
			if (xyPos.x >= 0 && xyPos.y >= 0)
			{
				tiles[xyPos.x][xyPos.y]->setFillColor(_tileTypeBinding[activeColorIndex]);
			}
			*/
			tiles[(mousePos.x - lateralOffset) / tileOffset][mousePos.y / tileOffset]->setFillColor(_tileTypeBinding[activeColorIndex]);
			// TODO possible improvement: draw a line between sample positions
			// to color every tile even when the mouse moves very fast
		}
	}
}

void Editor::draw(sf::RenderTarget& target)
{
	// draw tiles
	for (int x = 0; x < numTilesX; ++x)
	{
		for (int y = 0; y < numTilesY; ++y)
		{
			target.draw(*tiles[x][y]);
		}
	}
	// draw color/tile choices
	for (int y = 0; y < tileChoices.size(); ++y)
	{
		target.draw(*tileChoices[y]);
	}
	// draw item choices
	for (int y = 0; y < itemChoices.size(); ++y)
	{
		target.draw(*itemChoices[y]);
	}
}
