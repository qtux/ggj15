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
	// ---- normal items
	tileItemRects[1] = sf::IntRect(0,4*16,32,16);		// 1.  start (einmalig, nicht mehr auswaehlbar wenn schon gesetzt oder wird versetzt <- besser nicht einmalig wegen Mehrspieler? oder zwei Startitems)
	tileItemRects[2] = sf::IntRect(0,0,16,32);			// 2.  portal (goal)
	tileItemRects[3] = sf::IntRect(0,6*16,16,16);		// 3.  trigger
	tileItemRects[4] = sf::IntRect(0,5*16,16,16);		// 4.  coin
	tileItemRects[5] = sf::IntRect(0,3*16,16,16);		// 5.  clock
	tileItemRects[6] = sf::IntRect(0,2*16,16,16);		// 6.  key
	tileItemRects[7] = sf::IntRect(2*16,7*16,32,16);	// 7.  door
	// put in new items here (and adjust numbers below)
	// ---- deco item border
	tileItemRects[8] = sf::IntRect(16,5*16,16,16);		// 8.  mushroom (deco)
	tileItemRects[9] = sf::IntRect(16,6*16,16,16);		// 9.  flower (deco)
	tileItemRects[10] = sf::IntRect(0,7*16,16,16);		// 10. crystals (deco)
	tileItemRects[11] = sf::IntRect(16,7*16,16,16);		// 11. rock (deco)
	// put in new deco items here
	// ---- displayed item border
	tileItemRects[12] = sf::IntRect(2*16,4*16,16,32);	// 12. vertical door (not shown)
	// TODO more
	// TODO deco dialog with one item shown?	
	
	id[NOITEM] = 0;
	id[START] = 1;
	id[GOALPORTAL] = 2;
	id[TRIGGER] = 3;
	id[COIN] = 4;
	id[CLOCK] = 5;
	id[KEY] = 6;
	id[DOOR] = 7;
	// put in new items here (and adjust numbers below)
	// ---- deco item border
	id[DECO1] = 8;
	id[DECO2] = 9;
	id[DECO3] = 10;
	id[DECO4] = 11;
	// put in new deco items here
	// ---- displayed item border
	id[VERTICALDOOR] = 12;
	
	// large item texture parts
	bigItemRects[id[START]] = std::pair<sf::IntRect,sf::IntRect>(sf::IntRect(0,4*16,16,16), sf::IntRect(16,4*16,16,16));
	bigItemRects[id[GOALPORTAL]] = std::pair<sf::IntRect,sf::IntRect>(sf::IntRect(0,0,16,16), sf::IntRect(0,16,16,16));
	bigItemRects[id[DOOR]] = std::pair<sf::IntRect,sf::IntRect>(sf::IntRect(2*16,7*16,16,16), sf::IntRect(3*16,7*16,16,16));
	bigItemRects[id[VERTICALDOOR]] = std::pair<sf::IntRect,sf::IntRect>(sf::IntRect(2*16,4*16,16,16), sf::IntRect(2*16,5*16,16,16));
	
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
	infoText.setFont(font);
	infoText.setColor(sf::Color::Black);
	infoText.setCharacterSize(24);
	infoText.setPosition(lateralOffset/2, mapHeight + 45);
	standardHelpText = "Draw/Place Item: Left Click, Draw Color Box: Shift, Load Level: l, Save Level: s, Exit: ESC, Hide Help: h";
	infoText.setString(standardHelpText);
	
	// create drag tile
	mouseTile.setOutlineColor(sf::Color::Blue);
	mouseTile.setOutlineThickness(1.0f);
	mouseTile.setSize(sf::Vector2f(tileSize, tileSize));
	mouseTile.setPosition(0.5 * tileOffset, 1.0f);
	
	// initialize color editing with color 0 (mark in red)
	activeColorIndex = 0;
	activeItemIndex = -1;
	tileChoices[0]->setOutlineColor(sf::Color::Red);
	// initialize other variables
	pensize = 1;
	mousePressed = false;
	shiftActive = false;
	triggerMarkingActive = false;
	triggerMarked.first = -1;
	triggerMarked.second = -1;
	loadLevelActive = false;
	currentLevel = -1;
	overwrite = false;
	exit = false;
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

// TODO make item properties changeable
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
			infoText.setString(standardHelpText);
		}
		else if (overwrite)
		{
			overwrite = false;
			textOutput.setString("");
			infoText.setString(standardHelpText);
		}
		else
		{
			textOutput.setString("Really exit?");
			exit = true;
			infoText.setString("Confirm with y key, deny with n key.");
		}
	}
	
	// check for left mouseclick
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		if (!(loadLevelActive || overwrite))
		{
			textOutput.setString("");
			if (!shiftActive)
			{
				infoText.setString(standardHelpText);
			}
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
					activeItemIndex = getY(window);
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
					activeColorIndex = getY(window);
					tileChoices[activeColorIndex]->setOutlineColor(sf::Color::Red);
				}
			}
			
			// if mouse in gameboard
			if (mouseInGameboard(window))
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
							int x1 = std::min(markStart.x, (int) getX(window));
							int x2 = std::max(markStart.x, (int) getX(window));
							int y1 = std::min(markStart.y, (int) getY(window));
							int y2 = std::max(markStart.y, (int) getY(window));
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
							markStart.x = getX(window);
							markStart.y = getY(window);
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
						// get position
						int currentX = getX(window);
						int currentY = getY(window);
						
						// check if there was a big item and delete it's second part
						deleteBigItem(currentX, currentY);
						
						// set item
						setTexture(currentX, currentY, tileItemRects[activeItemIndex]);
						
						// apply necessary additions
						if (activeItemIndex == id[START] || activeItemIndex == id[GOALPORTAL]) // portal and start item
						{
							// if item would not fit, don't place it
							if (currentY + 1 >= numTilesY)
							{
								itemTiles[currentX][currentY]->setTextureRect(tileItemRects[id[NOITEM]]);
							}
							else
							{
								deleteBigItem(currentX, currentY+1);
								// place big item
								if (activeItemIndex == id[START])
								{
									itemTiles[currentX][currentY]->setTextureRect(bigItemRects[id[START]].first);
									setTexture(currentX, currentY+1, bigItemRects[id[START]].second);
								}
								else
								{
									itemTiles[currentX][currentY]->setTextureRect(bigItemRects[id[GOALPORTAL]].first);
									setTexture(currentX, currentY+1, bigItemRects[id[GOALPORTAL]].second);
								}
							}
						}
						if (activeItemIndex == id[TRIGGER]) // trigger item
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
							infoText.setString("Left click to mark two swap areas.");
						}
						if (activeItemIndex == id[DOOR]) // door item
						{
							// if door item would not fit, don't place it or place it vertically if that fits
							if (currentX + 1 >= numTilesX && currentY + 1 >= numTilesY)
							{
								itemTiles[currentX][currentY]->setTextureRect(tileItemRects[id[NOITEM]]);
							}
							else if (currentX + 1 >= numTilesX || itemChoices[id[DOOR]]->getTextureRect() == tileItemRects[id[VERTICALDOOR]])
							{
								deleteBigItem(currentX, currentY+1);
								itemTiles[currentX][currentY]->setTextureRect(bigItemRects[id[VERTICALDOOR]].first);
								setTexture(currentX, currentY+1, bigItemRects[id[VERTICALDOOR]].second);
							}
							else
							{
								deleteBigItem(currentX+1, currentY);
								itemTiles[currentX][currentY]->setTextureRect(bigItemRects[id[DOOR]].first);
								setTexture(currentX+1, currentY, bigItemRects[id[DOOR]].second);
							}
						}
						if (activeItemIndex >= id[DECO1] && activeItemIndex <= id[DECO4]) // deco item
						{
							// set to blocking
							decoItemBlocking[Key(currentX,currentY)] = 1;
							textOutput.setString("Deco item is blocking.");
							infoText.setString("Toggle blocking/non-blocking for decoration items with right click.");
						}
					}
					// if trigger marking is active, two correct markings must be drawn
					else
					{
						// TODO add safety so trigger item cannot be swapped (hero cannot be trapped immediately)?
						int xPos = getX(window);
						int yPos = getY(window);
						++marks;
						if (marks == 2)
						{
							triggerSwapPositionsX[triggerPos.x].second = xPos;
							triggerSwapPositionsY[triggerPos.y].second = yPos;
							triggerMarkingActive = false;
							markArea(xPos, yPos, sf::Color::Blue, quadrantSize);
							infoText.setString("Right click trigger to test swap areas, use r key to reset all triggers.");
						}
						else
						{
							triggerSwapPositionsX[triggerPos.x].first = xPos;
							triggerSwapPositionsY[triggerPos.y].first = yPos;
							infoText.setString("Left click to mark another swap area.");
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
			infoText.setString(standardHelpText);
		}
		if (!loadLevelActive && !triggerMarkingActive)
		{
			int xPos = getX(window);
			int yPos = getY(window);
			
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
			if (itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id[TRIGGER]])
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
				infoText.setString("Right click trigger to test swap areas, use r key to reset all triggers.");
			}
			
			// toggle door orientation if enough space
			/*if (itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id[DOOR]] && yPos + 1 < numTilesY)
			{
				itemTiles[xPos][yPos]->setTextureRect(tileItemRects[id[VERTICALDOOR]]);
				// swap dot
				setTexture(xPos, yPos+1, tileItemRects[13]);
				itemTiles[xPos+1][yPos]->setTextureRect(tileItemRects[id[NOITEM]]);
			}
			else if (itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id[VERTICALDOOR]] && xPos + 1 < numTilesX)
			{
				itemTiles[xPos][yPos]->setTextureRect(tileItemRects[id[DOOR]]);
				// swap dot
				setTexture(xPos+1, yPos, tileItemRects[13]);
				itemTiles[xPos][yPos+1]->setTextureRect(tileItemRects[id[NOITEM]]);
			}*/
			
			// toggle blocking for deco items
			if (itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id[DECO1]]	||
				itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id[DECO2]]		||
				itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id[DECO3]]	||
				itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id[DECO4]]			)
			{
				if (decoItemBlocking[Key(xPos,yPos)] == 1)
				{
					textOutput.setString("Deco item is non-blocking.");
					decoItemBlocking[Key(xPos,yPos)] = 0;
					infoText.setString("Toggle blocking/non-blocking for decoration items with right click.");
				}
				else
				{
					textOutput.setString("Deco item is blocking.");
					decoItemBlocking[Key(xPos,yPos)] = 1;
					infoText.setString("Toggle blocking/non-blocking for decoration items with right click.");
				}
			}
			
			// toggle door orientation
			// TODO do this differently or make sure nothing bad happens
			if (mouseTile.getTextureRect() == tileItemRects[id[DOOR]] || mouseTile.getTextureRect() == tileItemRects[id[VERTICALDOOR]])
			{
				if (itemChoices[id[DOOR]]->getTextureRect() == tileItemRects[id[VERTICALDOOR]])
				{
					itemChoices[id[DOOR]]->setTextureRect(tileItemRects[id[DOOR]]);
					mouseTile.setTextureRect(tileItemRects[id[DOOR]]);
				}
				else if (itemChoices[id[DOOR]]->getTextureRect() == tileItemRects[id[DOOR]])
				{
					itemChoices[id[DOOR]]->setTextureRect(tileItemRects[id[VERTICALDOOR]]);
					mouseTile.setTextureRect(tileItemRects[id[VERTICALDOOR]]);
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
			infoText.setString(standardHelpText);
		}
		if (!loadLevelActive && activeColorIndex != -1)
		{
			shiftActive = true;
			markStart.x = -1;
			markStart.y = -1;
			infoText.setString("Left click once to define origin, keep shift pressed, move the mouse and left click again to color a box.");
		}
		if (activeColorIndex == -1)
		{
			infoText.setString("Choose a color first to color a box.");
		}
	}
	
	// stop box coloring mode on shift release
	if (event.type == sf::Event::KeyReleased && (event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift))
	{
		infoText.setString(standardHelpText);
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
			infoText.setString("Confirm with y key, deny with n key (this will save it under a new name). Use ESC to cancel.");
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
			infoText.setString(standardHelpText);
		}
		if (exit)
		{
			textOutput.setString("");
			exit = false;
			infoText.setString(standardHelpText);
			return new Menu(Menu::Command::EDITOR);
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
			infoText.setString(standardHelpText);
		}
		if (exit)
		{
			textOutput.setString("");
			exit = false;
			infoText.setString(standardHelpText);
		}
	}
	
	// TODO prefer add and delete but somehow it does not work for me
	// increase pensize for drawing
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
	{
		if (pensize < 6)
		{
			++pensize;
		}
	}
	
	// decrease pensize for drawing
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
	{
		if (pensize > 1)
		{
			--pensize;
		}
	}
	
	// load when clicking l
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L)
	{
		loadLevelActive = true;
		currentLevel = 0;
		infoText.setString("Use left and right keys to choose a level, ESC to cancel loading.");
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
			infoText.setString(standardHelpText);
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
	
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H)
	{
		if (infoText.getCharacterSize() == 0)
		{
			infoText.setCharacterSize(24);
		}
		else
		{
			infoText.setCharacterSize(0);
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

void Editor::deleteBigItem(int x, int y)
{
	if (itemTiles[x][y]->getTextureRect() == bigItemRects[id[START]].first 		||
		itemTiles[x][y]->getTextureRect() == bigItemRects[id[GOALPORTAL]].first 	||
		itemTiles[x][y]->getTextureRect() == bigItemRects[id[VERTICALDOOR]].first		)
	{
		setTexture(x, y+1, tileItemRects[id[NOITEM]]);
	}
	if (itemTiles[x][y]->getTextureRect() == bigItemRects[id[DOOR]].first)
	{
		setTexture(x+1, y, tileItemRects[id[NOITEM]]);
	}
	if (itemTiles[x][y]->getTextureRect() == bigItemRects[id[START]].second 		||
		itemTiles[x][y]->getTextureRect() == bigItemRects[id[GOALPORTAL]].second 	||
		itemTiles[x][y]->getTextureRect() == bigItemRects[id[VERTICALDOOR]].second	)
	{
		setTexture(x, y-1, tileItemRects[id[NOITEM]]);
	}
	if (itemTiles[x][y]->getTextureRect() == bigItemRects[id[DOOR]].second)
	{
		setTexture(x-1, y, tileItemRects[id[NOITEM]]);
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
			// if mouse in gameboard
			if (mouseInGameboard(window))
			{
				int xPos = getX(window);
				int yPos = getY(window);
				for (int x = xPos; x < xPos + pensize; ++x)
				{
					for (int y = yPos; y < yPos + pensize; ++y)
					{
						if (x < numTilesX && y < numTilesY)
						{
							tiles[x][y]->setFillColor(tileColors[activeColorIndex]);
						}
					}
				}
			}
		}
		
		// show trigger marking
		if (triggerMarkingActive)
		{
			static int xPos;
			static int yPos;
			// delete old trigger marking
			markArea(xPos, yPos, sf::Color::Blue, quadrantSize);
			// draw new trigger marking
			xPos = getX(window);
			yPos = getY(window);
			markArea(xPos, yPos, sf::Color::Red, quadrantSize);
		}
		// drag items and colors
		else
		{
			// if mouse in gameboard
			if (mouseInGameboard(window))
			{
				int xPos = getX(window);
				int yPos = getY(window);
				// if color active
				if (activeColorIndex != -1)
				{
					int penTileSize = pensize * tileSize + (float)pensize*2;
					mouseTile.setSize(sf::Vector2f(penTileSize, penTileSize));
					mouseTile.setTexture(nullptr);
					mouseTile.setFillColor(tileColors[activeColorIndex]);
				}
				// if item active
				if (activeItemIndex != -1)
				{
					mouseTile.setFillColor(sf::Color::White);
					mouseTile.setTexture(&actionItemTexture);
					// show big items bigger
					if (activeItemIndex == id[DOOR] && itemChoices[activeItemIndex]->getTextureRect() != tileItemRects[id[VERTICALDOOR]])
					{
						mouseTile.setSize(sf::Vector2f(2*tileSize + 2.0f, tileSize));
						infoText.setString("Use right click to toggle door orientation before placing.");
					}
					else if (activeItemIndex == id[START] || activeItemIndex == id[GOALPORTAL] || activeItemIndex == id[DOOR])
					{
						mouseTile.setSize(sf::Vector2f(tileSize, 2*tileSize + 2.0f));
						if (activeItemIndex == id[DOOR])
						{
							infoText.setString("Use right click to toggle door orientation before placing.");
						}
					}
					else
					{
						mouseTile.setSize(sf::Vector2f(tileSize, tileSize));
					}
					mouseTile.setTextureRect(itemChoices[activeItemIndex]->getTextureRect());
				}
				// set position to mouse position
				mouseTile.setPosition(lateralOffset + xPos * tileOffset + 1.0f, yPos * tileOffset + 1.0f);
			}
		}
	}
	
	return this;
}

void Editor::draw(sf::RenderTarget& target, bool focus)
{
	target.draw(background);
	// draw tiles
	for (int x = 0; x < numTilesX; ++x)
	{
		for (int y = 0; y < numTilesY; ++y)
		{
			target.draw(*tiles[x][y]);
		}
	}
	// draw tile shown below the mouse
	target.draw(mouseTile);
	// draw itemTiles
	for (int x = 0; x < numTilesX; ++x)
	{
		for (int y = 0; y < numTilesY; ++y)
		{
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
	// draw text
	target.draw(textOutput);
	target.draw(infoText);
}

// TODO needs to be better code
// should directly create the list of files and find out which level is the highest number
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
				if (itemTiles[x][y]->getTextureRect() == bigItemRects[id[START]].first)
				{
					txtfile << "Start " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == bigItemRects[id[GOALPORTAL]].first)
				{
					txtfile << "Portal " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id[TRIGGER]])
				{
					txtfile << "TriggerItem " << x << " " << y << " " << triggerSwapPositionsX[x].first << " " << triggerSwapPositionsY[y].first << " " << triggerSwapPositionsX[x].second << " " << triggerSwapPositionsY[y].second << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id[COIN]])
				{
					txtfile << "Item CoinItem " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id[CLOCK]])
				{
					txtfile << "Item TimeItem " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id[KEY]])
				{
					txtfile << "Item KeyItem " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == bigItemRects[id[DOOR]].first)
				{
					// horizontal door
					txtfile << "Item DoorItem " << x << " " << y << " 0\n";
				}
				if (itemTiles[x][y]->getTextureRect() == bigItemRects[id[VERTICALDOOR]].first)
				{
					// vertical door
					txtfile << "Item DoorItem " << x << " " << y << " 1\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id[DECO1]])
				{
					txtfile << "Item DecorationItem " << x << " " << y << " " << decoItemBlocking[Key(x,y)] << " 16 80 16 16\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id[DECO2]])
				{
					txtfile << "Item DecorationItem " << x << " " << y << " " << decoItemBlocking[Key(x,y)] << " 16 96 16 16\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id[DECO3]])
				{
					txtfile << "Item DecorationItem " << x << " " << y << " " << decoItemBlocking[Key(x,y)] << " 0 112 16 16\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id[DECO4]])
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
	// delete all set items
	for (int x = 0; x < numTilesX; ++x)
	{
		for (int y = 0; y < numTilesY; ++y)
		{
			itemTiles[x][y]->setTextureRect(tileItemRects[id[NOITEM]]);
		}
	}
	
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
			setTexture(x, y, bigItemRects[id[START]].first);
			setTexture(x, y+1, bigItemRects[id[START]].second);
		}
		if (first == "Portal")
		{
			int x,y;
			iss >> x;
			iss >> y;
			setTexture(x, y, bigItemRects[id[GOALPORTAL]].first);
			setTexture(x, y+1, bigItemRects[id[GOALPORTAL]].second);
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
			setTexture(x, y, tileItemRects[id[TRIGGER]]);
			triggerSwapPositionsX[x] = std::pair<int, int>(x1,x2);
			triggerSwapPositionsY[y] = std::pair<int, int>(y1,y2);
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
				setTexture(x, y, tileItemRects[id[COIN]]);
			}
			if (second == "TimeItem")
			{
				setTexture(x, y, tileItemRects[id[CLOCK]]);
			}
			if (second == "KeyItem")
			{
				setTexture(x, y, tileItemRects[id[KEY]]);
			}
			if (second == "DoorItem")
			{
				// TODO do sth with closed?
				int vertical;
				iss >> vertical;
				if (vertical == 1)
				{
					setTexture(x, y, bigItemRects[id[VERTICALDOOR]].first);
					setTexture(x, y+1, bigItemRects[id[VERTICALDOOR]].second);
				}
				else
				{
					setTexture(x, y, bigItemRects[id[DOOR]].first);
					setTexture(x+1, y, bigItemRects[id[DOOR]].second);
				}
			}
			if (second == "DecorationItem")
			{
				int blocksPath;
				iss >> blocksPath;
				int texPosX, texPosY;
				iss >> texPosX;
				iss >> texPosY;
				// mushroom
				if (texPosX == 16 && texPosY == 80)
				{
					decoItemBlocking[Key(x,y)] = blocksPath;
					setTexture(x, y, tileItemRects[id[DECO1]]);
				}
				// flower
				if (texPosX == 16 && texPosY == 96)
				{
					decoItemBlocking[Key(x,y)] = blocksPath;
					setTexture(x, y, tileItemRects[id[DECO2]]);
				}
				// crystals
				if (texPosX == 0 && texPosY == 112)
				{
					decoItemBlocking[Key(x,y)] = blocksPath;
					setTexture(x, y, tileItemRects[id[DECO3]]);
				}
				// rock
				if (texPosX == 16 && texPosY == 112)
				{
					decoItemBlocking[Key(x,y)] = blocksPath;
					setTexture(x, y, tileItemRects[id[DECO4]]);
				}
			}
		}
	}
}

void Editor::setTexture(int x, int y, sf::IntRect textureRect)
{
	itemTiles[x][y]->setFillColor(sf::Color::White);
	itemTiles[x][y]->setTexture(&actionItemTexture);
	itemTiles[x][y]->setTextureRect(textureRect);
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

int Editor::getX(sf::RenderWindow& window)
{
	sf::Vector2f mousePos = getMouseWorldPos(window);
	return (mousePos.x - lateralOffset) / tileOffset;
}

int Editor::getY(sf::RenderWindow& window)
{
	sf::Vector2f mousePos = getMouseWorldPos(window);
	return mousePos.y / tileOffset;
}

bool Editor::mouseInGameboard(sf::RenderWindow& window)
{
	sf::Vector2f mousePos = getMouseWorldPos(window);
	return (mousePos.x >= lateralOffset && mousePos.y >= 0 && mousePos.x <= lateralOffset + mapWidth && mousePos.y <= mapHeight);
}
