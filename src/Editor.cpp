#include "Editor.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "global.hpp"
#include <iostream>
#include "Menu.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Editor::Editor():
	Scene({mapWidth + 2 * lateralOffset, mapHeight + lateralOffset}),
	actionItemTexture(gb::textureManager.getTexture("./img/items.png", false))
{
	// fill color choices array
	tileColors[0] = sf::Color::White;//0xffffffff;
	tileColors[1] = sf::Color(17, 148, 27);//0x11941bff;
	tileColors[2] = sf::Color(155, 109, 39);//0x9b6d27ff;
	tileColors[3] = sf::Color(150, 152, 150);//0x969896ff;
	tileColors[4] = sf::Color(95, 95, 95);//0x5f5f5fff;
	tileColors[5] = sf::Color(0, 1, 0);//0x000100ff;
	tileColors[6] = sf::Color(0, 0, 171);//0x0000abff;
	tileColors[7] = sf::Color(00, 62, 4);//0x003E04ff;
	
	// create tiles and itemTiles
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
			sf::RectangleShape* itemTile = new sf::RectangleShape();
			itemTile->setFillColor(sf::Color::Transparent);
			itemTile->setOutlineColor(sf::Color::Blue);
			itemTile->setOutlineThickness(1.0f);
			itemTile->setPosition(lateralOffset + x * tileOffset + 1.0f, y * tileOffset + 1.0f);
			itemTile->setSize(sf::Vector2f(tileSize, tileSize));
			itemTiles[x][y] = itemTile;
		}
	}
	
	// create color/tile choices bar
	for (int y = 0; y < tileChoices.size(); ++y)
	{
		sf::RectangleShape* tileChoice = new sf::RectangleShape();
		tileChoice->setFillColor(tileColors[y]);
		tileChoice->setOutlineColor(sf::Color::Black);
		tileChoice->setOutlineThickness(1.0f);
		tileChoice->setPosition(0.5 * tileOffset, y * tileOffset + 1.0f);
		tileChoice->setSize(sf::Vector2f(tileSize, tileSize));
		tileChoices[y] = tileChoice;
	}
	
	// load item rects
	tileItemRects[1] = sf::IntRect(0,4*16,32,16);	// 1. start (einmalig, nicht mehr auswaehlbar wenn schon gesetzt oder wird versetzt <- besser nicht einmalig wegen Mehrspieler? oder zwei Startitems)
	tileItemRects[2] = sf::IntRect(0,0,16,32);		// 2. portal (goal)
	tileItemRects[3] = sf::IntRect(0,6*16,16,16);	// 3. trigger
	tileItemRects[4] = sf::IntRect(0,5*16,16,16);	// 4. coin
	tileItemRects[5] = sf::IntRect(0,3*16,16,16);	// 5. clock
	tileItemRects[6] = sf::IntRect(0,2*16,16,16);	// 6. key
	// TODO more
	// TODO deco dialog with one item shown
	
	// create item choices bar
	for (int y = 0; y < itemChoices.size(); ++y)
	{
		sf::RectangleShape* itemChoice = new sf::RectangleShape();
		itemChoice->setOutlineColor(sf::Color::Black);
		itemChoice->setTexture(&actionItemTexture);
		itemChoice->setTextureRect(tileItemRects[y]);
		itemChoice->setOutlineThickness(1.0f);
		itemChoice->setPosition(lateralOffset + mapWidth + tileOffset, y * tileOffset + 1.0f);
		itemChoice->setSize(sf::Vector2f(tileSize, tileSize));
		itemChoices[y] = itemChoice;
	}
	
	// initialize color editing with color 0 (mark in red), no item editing, no mouse or key pressed, no item active
	activeColorIndex = 0;
	activeItemIndex = -1;
	tileChoices[0]->setOutlineColor(sf::Color::Red);
	mousePressed = false;
	shiftActive = false;
	triggerMarkingActive = false;
}

Editor::~Editor()
{
	for (int x = 0; x < numTilesX; ++x)
	{
		for (int y = 0; y < numTilesY; ++y)
		{
			delete(tiles[x][y]);
			delete(itemTiles[x][y]);
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
// TODO make item properties changeable
// TODO change pen size depending on item set (e.g. Portal, Door)
// TODO make trigger test possible so one can see the result
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
				// if box coloring is active
				if (shiftActive)
				{
					// color chosen tiles in the box
					if (markStart.x != -1 && markStart.y != -1)
					{
						// find left upper corner and right lower corner of the box
						int x1 = std::min(markStart.x, (int) (mousePos.x - lateralOffset) / tileOffset);
						int x2 = std::max(markStart.x, (int) (mousePos.x - lateralOffset) / tileOffset);
						int y1 = std::min(markStart.y, (int) mousePos.y / tileOffset);
						int y2 = std::max(markStart.y, (int) mousePos.y / tileOffset);
						// color the box
						for (int x = x1; x <= x2; ++x)
						{
							for (int y = y1; y <= y2; ++y)
							{
								tiles[x][y]->setFillColor(tileColors[activeColorIndex]);
							}
						}
					}
					// color origin tile of box coloring
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
					int currentX = (mousePos.x - lateralOffset) / tileOffset;
					int currentY = mousePos.y / tileOffset;
					itemTiles[currentX][currentY]->setFillColor(sf::Color::White);
					itemTiles[currentX][currentY]->setTexture(&actionItemTexture);
					itemTiles[currentX][currentY]->setTextureRect(tileItemRects[activeItemIndex]);
					if (activeItemIndex == 3) // trigger item
					{
						// force marking of quadrants
						triggerMarkingActive = true;
						// number of markings
						marks = 0;
						// trigger position
						triggerPos = sf::Vector2f(currentX, currentY);
						// initialize map position
						std::pair<int, int> swapTilesPosX;
						std::pair<int, int> swapTilesPosY;
						triggerSwapPositionsX[currentX] = swapTilesPosX;
						triggerSwapPositionsY[currentY] = swapTilesPosY;
					}
				}
				// if trigger marking is active, two correct markings must be drawn
				else
				{
					// TODO add safety so trigger item cannot be swapped (hero cannot be trapped immediately)?
					int xPos = (mousePos.x - lateralOffset) / tileOffset;
					int yPos = mousePos.y / tileOffset;
					++marks;
					if (marks == 2)
					{
						triggerSwapPositionsX[triggerPos.x].second = xPos;
						triggerSwapPositionsY[triggerPos.y].second = yPos;
						triggerMarkingActive = false;
						if (xPos >= 0 && yPos >= 0 && xPos + quadrantSize <= numTilesX && yPos + quadrantSize <= numTilesY) {
							for (int x = xPos; x < xPos + quadrantSize; ++x)
							{
								for (int y = yPos; y < yPos + quadrantSize; ++y)
								{
									itemTiles[x][y]->setOutlineColor(sf::Color::Blue);
								}
							}
						}
					}
					else
					{
						triggerSwapPositionsX[triggerPos.x].first = xPos;
						triggerSwapPositionsY[triggerPos.y].first = yPos;
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
		// show trigger marking
		if (triggerMarkingActive)
		{
			static int xPos;
			static int yPos;
			// delete old trigger marking
			if (xPos >= 0 && yPos >= 0 && xPos + quadrantSize <= numTilesX && yPos + quadrantSize <= numTilesY) {
				for (int x = xPos; x < xPos + quadrantSize; ++x)
				{
					for (int y = yPos; y < yPos + quadrantSize; ++y)
					{
						itemTiles[x][y]->setOutlineColor(sf::Color::Blue);
					}
				}
			}
			// draw new trigger marking
			xPos = (mousePos.x - lateralOffset) / tileOffset;
			yPos = mousePos.y / tileOffset;
			if (xPos >= 0 && yPos >= 0 && xPos + quadrantSize <= numTilesX && yPos + quadrantSize <= numTilesY) {
				for (int x = xPos; x < xPos + quadrantSize; ++x)
				{
					for (int y = yPos; y < yPos + quadrantSize; ++y)
					{
						itemTiles[x][y]->setOutlineColor(sf::Color::Red);
					}
				}
			}
		}
	}
	
	// box coloring on shift click (only activate here and set start tile)
	if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift))
	{
		shiftActive = true;
		markStart.x = -1;
		markStart.y = -1;
	}
	
	// stop box coloring mode on shift release
	if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift))
	{
		shiftActive = false;
		// cleanup non-colored tiles
		if (markStart.x != -1 && markStart.y != -1)
		{
			tiles[markStart.x][markStart.y]->setOutlineThickness(1.0f);
			tiles[markStart.x][markStart.y]->setOutlineColor(sf::Color::Blue);
		}
		markStart.x = -1;
		markStart.y = -1;
	}
	
	// save when clicking s
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
	{
		saveLevel();
	}
	
	return this;
}

// TODO possible improvement: draw a line between sample positions to color every tile even when the mouse moves very fast
Scene* Editor::update(sf::Time deltaT, sf::RenderWindow& window)
{
	// if mouse pressed to draw and shift is not pressed and color mode is active
	// i.e. continuously draw color on left click + mouse move
	if (mousePressed && !shiftActive && activeColorIndex != -1)
	{
		sf::Vector2f mousePos = getMouseWorldPos(window);
		// if mouse in gameboard
		if (mousePos.x >= lateralOffset && mousePos.y >= 0 && mousePos.x <= lateralOffset + mapWidth && mousePos.y <= mapHeight)
		{
			tiles[(mousePos.x - lateralOffset) / tileOffset][mousePos.y / tileOffset]->setFillColor(tileColors[activeColorIndex]);
		}
	}
	return this;
}

void Editor::draw(sf::RenderTarget& target, bool focus)
{
	target.draw(background);
	// draw tiles and itemTiles
	for (int x = 0; x < numTilesX; ++x)
	{
		for (int y = 0; y < numTilesY; ++y)
		{
			target.draw(*tiles[x][y]);
			target.draw(*itemTiles[x][y]);
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

// TODO check out which number the level shall have to not overwrite existing ones
// needs to be better code
// needs to register new levels so they can be played
// should directly create the list of files and find out which level is the highest number
// play around with overwriting level and not overwriting level currently edited
void Editor::saveLevel()
{
	// determine level number
	std::string indexFile = "levels/index.txt";
	// read level number from file
	std::ifstream infile(indexFile);
	std::string line;
	std::vector<int> levelNumbers;
	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			std::istringstream iss(line);
			int current;
			iss >> current;
			levelNumbers.push_back(current);
			std::cout << current << std::endl;
		}
	}
	infile.close();
	// search maximum
	int maxLevel = 0;
	for (int i = 0; i < levelNumbers.size(); i++)
	{
		maxLevel = std::max(maxLevel, levelNumbers[i]);
	}
	maxLevel++;
	// write new maxLevel number into file
	std::ofstream outfile;
	outfile.open(indexFile, std::ios_base::app);
	if (outfile.is_open())
	{
		outfile << maxLevel << "\n";
	}
	
	// level name png
	std::stringstream streamPNG;
	streamPNG << "levels/level" << maxLevel << ".png";
	
	// save colors in png image
	sf::Image level;
	level.create(30, 24, sf::Color::White);
	for (int x = 0; x < numTilesX; ++x)
	{
		for (int y = 0; y < numTilesY; ++y)
		{
			level.setPixel(x, y, tiles[x][y]->getFillColor());
		}
	}
	if (!level.saveToFile(streamPNG.str()))
	{
		//TODO show user
		std::cout << "something went wrong creating png file" << std::endl;
	}
	
	// level name txt
	std::stringstream streamTXT;
	streamTXT << "levels/level" << maxLevel << ".txt";
	
	// save items in txt file
	std::ofstream txtfile(streamTXT.str());
	if (txtfile.is_open())
	{
		for (int x = 0; x < numTilesX; ++x)
		{
			for (int y = 0; y < numTilesY; ++y)
			{
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[1])
				{
					txtfile << "Start " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[2])
				{
					txtfile << "Portal " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[3])
				{
					txtfile << "TriggerItem " << x << " " << y << " " << triggerSwapPositionsX[x].first << " " << triggerSwapPositionsY[y].first << " " << triggerSwapPositionsX[x].second << " " << triggerSwapPositionsY[y].second << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[4])
				{
					txtfile << "Item CoinItem " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[5])
				{
					txtfile << "Item TimeItem " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[6])
				{
					txtfile << "Item KeyItem " << x << " " << y << "\n";
				}
			}
		}
	}
	else
	{
		//TODO show user
		std::cout << "something went wrong creating txt file" << std::endl;
	}
}

sf::Vector2f Editor::getMouseWorldPos(sf::RenderWindow& window)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	return window.mapPixelToCoords(pixelPos);
}
