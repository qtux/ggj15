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
	tileItemRects[1] = sf::IntRect(0,4*16,32,16);		// 1.  start (einmalig, nicht mehr auswaehlbar wenn schon gesetzt oder wird versetzt <- besser nicht einmalig wegen Mehrspieler? oder zwei Startitems)
	tileItemRects[2] = sf::IntRect(0,0,16,32);			// 2.  portal (goal)
	tileItemRects[3] = sf::IntRect(0,6*16,16,16);		// 3.  trigger
	tileItemRects[4] = sf::IntRect(0,5*16,16,16);		// 4.  coin
	tileItemRects[5] = sf::IntRect(0,3*16,16,16);		// 5.  clock
	tileItemRects[6] = sf::IntRect(0,2*16,16,16);		// 6.  key
	tileItemRects[7] = sf::IntRect(2*16,7*16,32,16);	// 7.  door
	tileItemRects[8] = sf::IntRect(16,5*16,16,16);		// 8.  mushroom (deco)
	tileItemRects[9] = sf::IntRect(16,6*16,16,16);		// 9.  flower (deco)
	tileItemRects[10] = sf::IntRect(0,7*16,16,16);		// 10. crystals (deco)
	tileItemRects[11] = sf::IntRect(16,7*16,16,16);		// 11. rocks(deco)
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
	
	// read level numbers from file
	std::ifstream indexFile("levels/index.txt");
	if (indexFile.is_open())
	{
		int level;
		while (indexFile >> level)
		{
			levels.push_back(level);
		}
	}
	std::sort(levels.begin(), levels.end());
	std::unique(levels.begin(), levels.end());
	
	// load font
	font.loadFromFile("./fonts/LiberationSerif-Regular.ttf");
	// initialize text
	textOutput.setFont(font);
	textOutput.setColor(sf::Color::Black);
	textOutput.setCharacterSize(32);
	textOutput.setPosition(mapWidth/2, mapHeight + 5); // x = Lateraloffset?
	textOutput.setString("");
	
	// initialize color editing with color 0 (mark in red), no item editing, no mouse or key pressed, no item active
	activeColorIndex = 0;
	activeItemIndex = -1;
	tileChoices[0]->setOutlineColor(sf::Color::Red);
	mousePressed = false;
	shiftActive = false;
	triggerMarkingActive = false;
	triggerMarked.first = -1;
	triggerMarked.second = -1;
	loadLevelActive = false;
	currentLevel = -1;
	overwrite = false;
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
		if (loadLevelActive)
		{
			loadLevelActive = false;
			textOutput.setString("");
		}
		else
		{
			return new Menu(Menu::Command::EDITOR);
		}
	}
	
	// check for left mouseclick
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		if (!(loadLevelActive || overwrite))
		{
			textOutput.setString("");
		}
		if (!loadLevelActive)
		{
			mousePressed = true;
			sf::Vector2f mousePos = getMouseWorldPos(window);
			
			// if trigger was marked (i.e. chosen), unmark it
			if (triggerMarked.first != -1 && triggerMarked.second != -1)
			{
				markArea(triggerSwapPositionsX[triggerMarked.first].first, triggerSwapPositionsY[triggerMarked.second].first, sf::Color::Blue, quadrantSize);
				markArea(triggerSwapPositionsX[triggerMarked.first].second, triggerSwapPositionsY[triggerMarked.second].second, sf::Color::Blue, quadrantSize);
				triggerMarked.first = -1;
				triggerMarked.second = -1;
			}
			
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
						if (activeItemIndex > 7 && activeItemIndex < 12) // deco item
						{
							// set to blocking
							decoItemBlocking[Key(currentX,currentY)] = 1;
							textOutput.setString("Deco item is blocking.");
						}
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
							markArea(currentX, currentY, sf::Color::Red, quadrantSize);
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
							markArea(xPos, yPos, sf::Color::Blue, quadrantSize);
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
	}
		
	if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{
		mousePressed = false;
	}
	
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
	{
		if (!(loadLevelActive || overwrite))
		{
			textOutput.setString("");
		}
		if (!loadLevelActive && !triggerMarkingActive)
		{
			sf::Vector2f mousePos = getMouseWorldPos(window);
			int xPos = (mousePos.x - lateralOffset) / tileOffset;
			int yPos = mousePos.y / tileOffset;
			
			// if trigger was marked (i.e. chosen), unmark it
			if (triggerMarked.first != -1 && triggerMarked.second != -1)
			{
				markArea(triggerSwapPositionsX[triggerMarked.first].first, triggerSwapPositionsY[triggerMarked.second].first, sf::Color::Blue, quadrantSize);
				markArea(triggerSwapPositionsX[triggerMarked.first].second, triggerSwapPositionsY[triggerMarked.second].second, sf::Color::Blue, quadrantSize);
				triggerMarked.first = -1;
				triggerMarked.second = -1;
			}
			
			// TODO swapped triggers may be outlined yellow or sth (probably)
			// if trigger item is on right clicked tile
			if (itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[3])
			{
				// mark swapped tiles
				markArea(triggerSwapPositionsX[xPos].first, triggerSwapPositionsY[yPos].first, sf::Color::Red, quadrantSize);
				markArea(triggerSwapPositionsX[xPos].second, triggerSwapPositionsY[yPos].second, sf::Color::Red, quadrantSize);
				triggerMarked.first = xPos;
				triggerMarked.second = yPos;
				// swap colors of both quadrants (simulate trigger)
				sf::Color storeColor;
				for (int x = 0; x < quadrantSize; ++x)
				{
					for (int y = 0; y < quadrantSize; ++y)
					{
						storeColor = tiles[triggerSwapPositionsX[xPos].first+x][triggerSwapPositionsY[yPos].first+y]->getFillColor();
						tiles[triggerSwapPositionsX[xPos].first+x][triggerSwapPositionsY[yPos].first+y]->setFillColor(tiles[triggerSwapPositionsX[xPos].second+x][triggerSwapPositionsY[yPos].second+y]->getFillColor());
						tiles[triggerSwapPositionsX[xPos].second+x][triggerSwapPositionsY[yPos].second+y]->setFillColor(storeColor);
					}
				}
				triggerStack.push(std::pair<int, int>(xPos, yPos));
			}
			
			// toggle door orientation
			if (itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[7])
			{
				itemTiles[xPos][yPos]->setTextureRect(sf::IntRect(2*16,4*16,16,32));
			}
			else if (itemTiles[xPos][yPos]->getTextureRect() == sf::IntRect(2*16,4*16,16,32))
			{
				itemTiles[xPos][yPos]->setTextureRect(tileItemRects[7]);
			}
			
			// toggle blocking for deco items
			if (itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[8]  ||
				itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[9]  ||
				itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[10] ||
				itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[11]   )
			{
				if (decoItemBlocking[Key(xPos,yPos)] == 1)
				{
					textOutput.setString("Deco item is non-blocking.");
					decoItemBlocking[Key(xPos,yPos)] = 0;
				}
				else
				{
					textOutput.setString("Deco item is blocking.");
					decoItemBlocking[Key(xPos,yPos)] = 1;
				}
			}
		}
	}
	
	// box coloring on shift click (only activate here and set start tile)
	if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift))
	{
		if (!(loadLevelActive || overwrite))
		{
			textOutput.setString("");
		}
		if (!loadLevelActive)
		{
			shiftActive = true;
			markStart.x = -1;
			markStart.y = -1;
		}
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
		resetTriggers();
		// if the level is not a new one but was loaded or saved already let user choose to overwrite or not
		if (currentLevel != -1)
		{
			// tell user which level will be overwritten
			textOutput.setString("Overwrite Level " + std::to_string(currentLevel) + "?");
			overwrite = true;
		}
		else
		{
			saveLevel(false);
		}
	}
	
	// overwrite level
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Y)
	{
		if (overwrite)
		{
			textOutput.setString("");
			overwrite = false;
			saveLevel(true);
		}
	}
	
	// do not overwrite, i.e. save normally
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::N)
	{
		if (overwrite)
		{
			textOutput.setString("");
			overwrite = false;
			saveLevel(false);
		}
	}
	
	// load when clicking l
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L)
	{
		loadLevelActive = true;
		currentLevel = 0;
	}
	
	// reset triggers when clicking r
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
	{
		resetTriggers();
	}
	
	// load level on enter
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
	{
		if (loadLevelActive)
		{
			loadLevel(currentLevel);
			loadLevelActive = false;
			textOutput.setString("");
		}
	}
	
	// select level
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
	{
		if (loadLevelActive)
		{
			currentLevel = levels[nextPos(currentLevel, levels.size(), false)];
		}
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
	{
		if (loadLevelActive)
		{
			currentLevel = levels[nextPos(currentLevel, levels.size(), true)];
		}
	}
	
	return this;
}

void Editor::markArea(int xPos, int yPos, sf::Color color, int quadrantSize)
{
	// if marking is in gameboard
	if (xPos >= 0 && yPos >= 0 && xPos + quadrantSize <= numTilesX && yPos + quadrantSize <= numTilesY) {
		// mark given size * given size tiles in given color
		for (int x = xPos; x < xPos + quadrantSize; ++x)
		{
			for (int y = yPos; y < yPos + quadrantSize; ++y)
			{
				itemTiles[x][y]->setOutlineColor(color);
			}
		}
	}
}

void Editor::resetTriggers()
{
	// if trigger was marked (i.e. chosen), unmark it
	if (triggerMarked.first != -1 && triggerMarked.second != -1)
	{
		markArea(triggerSwapPositionsX[triggerMarked.first].first, triggerSwapPositionsY[triggerMarked.second].first, sf::Color::Blue, quadrantSize);
		markArea(triggerSwapPositionsX[triggerMarked.first].second, triggerSwapPositionsY[triggerMarked.second].second, sf::Color::Blue, quadrantSize);
		triggerMarked.first = -1;
		triggerMarked.second = -1;
	}
		
	// swap triggers back in correct order
	while (!triggerStack.empty())
	{
		int xPos = triggerStack.top().first;
		int yPos = triggerStack.top().second;
		triggerStack.pop();
		sf::Color storeColor;
		for (int x = 0; x < quadrantSize; ++x)
		{
			for (int y = 0; y < quadrantSize; ++y)
			{
				storeColor = tiles[triggerSwapPositionsX[xPos].first+x][triggerSwapPositionsY[yPos].first+y]->getFillColor();
				tiles[triggerSwapPositionsX[xPos].first+x][triggerSwapPositionsY[yPos].first+y]->setFillColor(tiles[triggerSwapPositionsX[xPos].second+x][triggerSwapPositionsY[yPos].second+y]->getFillColor());
				tiles[triggerSwapPositionsX[xPos].second+x][triggerSwapPositionsY[yPos].second+y]->setFillColor(storeColor);
			}
		}
	}
}

// TODO possible improvement: draw a line between sample positions to color every tile even when the mouse moves very fast
Scene* Editor::update(sf::Time deltaT, sf::RenderWindow& window)
{	
	if (loadLevelActive)
	{
		textOutput.setString("Level " + std::to_string(currentLevel));
	}
	else
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
		
		// show trigger marking
		if (triggerMarkingActive)
		{
			sf::Vector2f mousePos = getMouseWorldPos(window);
			static int xPos;
			static int yPos;
			// delete old trigger marking
			markArea(xPos, yPos, sf::Color::Blue, quadrantSize);
			// draw new trigger marking
			xPos = (mousePos.x - lateralOffset) / tileOffset;
			yPos = mousePos.y / tileOffset;
			markArea(xPos, yPos, sf::Color::Red, quadrantSize);
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
	target.draw(textOutput);
}

// TODO check out which number the level shall have to not overwrite existing ones
// needs to be better code
// needs to register new levels so they can be played
// should directly create the list of files and find out which level is the highest number
// play around with overwriting level and not overwriting level currently edited
void Editor::saveLevel(bool overwrite)
{
	// if saved for the first time and nothing was loaded yet or if user does not want overwrite
	if (!overwrite)
	{
		// create new currentLevel
		currentLevel = levels.back() + 1;
		// write new maxLevel number into file
		std::string indexFile = "levels/index.txt";
		std::ofstream outfile;
		outfile.open(indexFile, std::ios_base::app);
		if (outfile.is_open())
		{
			outfile << currentLevel << "\n";
		}
	}
	
	// level name png
	std::stringstream streamPNG;
	streamPNG << "levels/level" << currentLevel << ".png";
	
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
	streamTXT << "levels/level" << currentLevel << ".txt";
	
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
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[7])
				{
					// horizontal door
					txtfile << "Item DoorItem " << x << " " << y << " 0\n";
				}
				if (itemTiles[x][y]->getTextureRect() == sf::IntRect(2*16,4*16,16,32))
				{
					// vertical door
					txtfile << "Item DoorItem " << x << " " << y << " 1\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[8])
				{
					txtfile << "Item DecorationItem " << x << " " << y << " " << decoItemBlocking[Key(x,y)] << " 16 80 16 16\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[9])
				{
					txtfile << "Item DecorationItem " << x << " " << y << " " << decoItemBlocking[Key(x,y)] << " 16 96 16 16\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[10])
				{
					txtfile << "Item DecorationItem " << x << " " << y << " " << decoItemBlocking[Key(x,y)] << " 0 112 16 16\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[11])
				{
					txtfile << "Item DecorationItem " << x << " " << y << " " << decoItemBlocking[Key(x,y)] << " 16 112 16 16\n";
				}
			}
		}
	}
	else
	{
		//TODO show user
		std::cout << "something went wrong creating txt file" << std::endl;
	}
	// tell user where the level was saved
	textOutput.setString("Saved as Level " + std::to_string(currentLevel));
}

void Editor::loadLevel(int level)
{
	// load colors
	sf::Image levelPNG;
	levelPNG.loadFromFile("./levels/level" + std::to_string(level) + ".png");
	for (int x = 0; x < numTilesX; ++x)
	{
		for (int y = 0; y < numTilesY; ++y)
		{
			tiles[x][y]->setFillColor(levelPNG.getPixel(x, y));
		}
	}
	
	// load items
	std::ifstream infile("./levels/level" + std::to_string(level) + ".txt");
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string first;
		iss >> first;
		if (first == "Start")
		{
			int x,y;
			iss >> x;
			iss >> y;
			itemTiles[x][y]->setFillColor(sf::Color::White);
			itemTiles[x][y]->setTexture(&actionItemTexture);
			itemTiles[x][y]->setTextureRect(tileItemRects[1]);
		}
		if (first == "Portal")
		{
			int x,y;
			iss >> x;
			iss >> y;
			itemTiles[x][y]->setFillColor(sf::Color::White);
			itemTiles[x][y]->setTexture(&actionItemTexture);
			itemTiles[x][y]->setTextureRect(tileItemRects[2]);
		}
		if (first == "TriggerItem")
		{
			int x, y, x1, x2, y1, y2;
			iss >> x;
			iss >> y;
			iss >> x1;
			iss >> y1;
			iss >> x2;
			iss >> y2;
			itemTiles[x][y]->setFillColor(sf::Color::White);
			itemTiles[x][y]->setTexture(&actionItemTexture);
			itemTiles[x][y]->setTextureRect(tileItemRects[3]);
			triggerSwapPositionsX[x] = std::pair<int, int>(x1,y1);
			triggerSwapPositionsY[y] = std::pair<int, int>(x2,y2);
		}
		if (first == "Item")
		{
			std::string second;
			iss >> second;
			int x,y;
			iss >> x;
			iss >> y;
			if (second == "CoinItem")
			{
				itemTiles[x][y]->setFillColor(sf::Color::White);
				itemTiles[x][y]->setTexture(&actionItemTexture);
				itemTiles[x][y]->setTextureRect(tileItemRects[4]);
			}
			if (second == "TimeItem")
			{
				itemTiles[x][y]->setFillColor(sf::Color::White);
				itemTiles[x][y]->setTexture(&actionItemTexture);
				itemTiles[x][y]->setTextureRect(tileItemRects[5]);
			}
			if (second == "KeyItem")
			{
				itemTiles[x][y]->setFillColor(sf::Color::White);
				itemTiles[x][y]->setTexture(&actionItemTexture);
				itemTiles[x][y]->setTextureRect(tileItemRects[6]);
			}
			if (second == "DoorItem")
			{
				// TODO do sth with closed?
				int vertical;
				iss >> vertical;
				itemTiles[x][y]->setFillColor(sf::Color::White);
				itemTiles[x][y]->setTexture(&actionItemTexture);
				if (vertical == 1)
				{
					itemTiles[x][y]->setTextureRect(sf::IntRect(2*16,4*16,16,32));
				}
				else
				{
					itemTiles[x][y]->setTextureRect(tileItemRects[7]);
				}
			}
			if (second == "DecorationItem")
			{
				// TODO add block or non-block choice to item
				int blocksPath;
				iss >> blocksPath;
				int texPosX, texPosY;
				iss >> texPosX;
				iss >> texPosY;
				// mushroom
				if (texPosX == 16 && texPosY == 80)
				{
					decoItemBlocking[Key(x,y)] = blocksPath;
					itemTiles[x][y]->setFillColor(sf::Color::White);
					itemTiles[x][y]->setTexture(&actionItemTexture);
					itemTiles[x][y]->setTextureRect(tileItemRects[8]);
				}
				// flower
				if (texPosX == 16 && texPosY == 96)
				{
					decoItemBlocking[Key(x,y)] = blocksPath;
					itemTiles[x][y]->setFillColor(sf::Color::White);
					itemTiles[x][y]->setTexture(&actionItemTexture);
					itemTiles[x][y]->setTextureRect(tileItemRects[9]);
				}
				// crystals
				if (texPosX == 0 && texPosY == 112)
				{
					decoItemBlocking[Key(x,y)] = blocksPath;
					itemTiles[x][y]->setFillColor(sf::Color::White);
					itemTiles[x][y]->setTexture(&actionItemTexture);
					itemTiles[x][y]->setTextureRect(tileItemRects[10]);
				}
				// rock
				if (texPosX == 16 && texPosY == 112)
				{
					decoItemBlocking[Key(x,y)] = blocksPath;
					itemTiles[x][y]->setFillColor(sf::Color::White);
					itemTiles[x][y]->setTexture(&actionItemTexture);
					itemTiles[x][y]->setTextureRect(tileItemRects[11]);
				}
			}
		}
	}
}

int Editor::nextPos(int pos, int size, bool clockWise)
{
	auto forward = (pos < size - 1) ? pos + 1 : 0;
	auto backward = (pos > 0) ? pos - 1 : size - 1;
	return clockWise ? forward : backward;
}

sf::Vector2f Editor::getMouseWorldPos(sf::RenderWindow& window)
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	return window.mapPixelToCoords(pixelPos);
}
