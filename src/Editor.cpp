#include "Editor.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "global.hpp"
#include <iostream>
#include "Menu.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

// TODO make time changeable -> Timeout (Timebuff -> apply for every clock?)
// TODO include text
Editor::Editor():
	Scene({mapWidth + 2 * lateralOffset, mapHeight + lateralOffset}),
	actionItemTexture(gb::ressourceManager.getTexture("./img/items.png", false)),
	id ({
	// ---- normal items
	{NOITEM, 0},
	{START, 1},
	{GOAL, 2},
	{TRIGGER, 3},
	{COIN, 4},
	{CLOCK, 5},
	{KEY, 6},
	{DOOR, 7},
	{DOORSWITCH, 8},
	// put in new items here (and adjust numbers below)
	// ---- deco item border
	{DECO1, 9},
	{DECO2, 10},
	{DECO3, 11},
	{DECO4, 12},
	// put in new deco items here
	// ---- npc border
	{SLIME, 13},
	// put in new npcs here (and adjust numbers below)
	// ---- non-displayed item border
	{VERTICALDOOR, 14}
	})
{
	// fill color choices array
	tileColors[0] = sf::Color::White;//0xffffffff;
	tileColors[1] = sf::Color(17, 148, 27);//0x11941bff;
	tileColors[2] = sf::Color(155, 109, 39);//0x9b6d27ff;
	tileColors[3] = sf::Color(150, 152, 150);//0x969896ff;
	tileColors[4] = sf::Color(95, 95, 95);//0x5f5f5fff;
	tileColors[5] = sf::Color(0, 1, 0);//0x000100ff;
	tileColors[6] = sf::Color(0, 0, 171);//0x0000abff;
	tileColors[7] = sf::Color(0, 62, 4);//0x003E04ff;
	
	
	// create tiles and itemTiles (gameboard)
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
	tileItemRects[id.at(START)] = sf::IntRect(0,4*16,32,16);			// 1.  start (einmalig, nicht mehr auswaehlbar wenn schon gesetzt oder wird versetzt <- besser nicht einmalig wegen Mehrspieler? oder zwei Startitems)
	tileItemRects[id.at(GOAL)] = sf::IntRect(0,0,16,32);				// 2.  portal (goal)
	tileItemRects[id.at(TRIGGER)] = sf::IntRect(0,6*16,16,16);			// 3.  trigger
	tileItemRects[id.at(COIN)] = sf::IntRect(0,5*16,16,16);				// 4.  coin
	tileItemRects[id.at(CLOCK)] = sf::IntRect(0,3*16,16,16);			// 5.  clock
	tileItemRects[id.at(KEY)] = sf::IntRect(0,2*16,16,16);				// 6.  key
	tileItemRects[id.at(DOOR)] = sf::IntRect(2*16,7*16,32,16);			// 7.  door
	tileItemRects[id.at(DOORSWITCH)] = sf::IntRect(2*16,3*16,16,16);	// 8.  doorswitch
	// put in new items here (and adjust numbers above)
	// ---- deco item border
	tileItemRects[id.at(DECO1)] = sf::IntRect(16,5*16,16,16);			// 1.(deco)	mushroom
	tileItemRects[id.at(DECO2)] = sf::IntRect(16,6*16,16,16);			// 2.(deco)	flower
	tileItemRects[id.at(DECO3)] = sf::IntRect(0,7*16,16,16);			// 3.(deco)	crystals
	tileItemRects[id.at(DECO4)] = sf::IntRect(16,7*16,16,16);			// 4.(deco)	rock
	// put in new deco items here
	// ---- npc border
	tileItemRects[id.at(SLIME)] = sf::IntRect(3*16,0,16,16);			// 1.(npc)	slime
	// put in new npcs here (and adjust numbers above)
	// ---- non-displayed item border
	tileItemRects[id.at(VERTICALDOOR)] = sf::IntRect(2*16,4*16,16,32);	// vertical door (not shown)
	// TODO more
	// TODO deco dialog with one item shown?	
	
	// ---- normal items
	/*id[NOITEM] = 0;
	id[START] = 1;
	id[GOAL] = 2;
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
	// ---- npc border
	id[SLIME] = 12;
	// put in new npcs here (and adjust numbers below)
	// ---- non-displayed item border
	id[VERTICALDOOR] = 13;*/
	
	// large item texture parts
	bigItemRects[id.at(START)] = std::pair<sf::IntRect,sf::IntRect>(sf::IntRect(0,4*16,16,16), sf::IntRect(16,4*16,16,16));
	bigItemRects[id.at(GOAL)] = std::pair<sf::IntRect,sf::IntRect>(sf::IntRect(0,0,16,16), sf::IntRect(0,16,16,16));
	bigItemRects[id.at(DOOR)] = std::pair<sf::IntRect,sf::IntRect>(sf::IntRect(2*16,7*16,16,16), sf::IntRect(3*16,7*16,16,16));
	bigItemRects[id.at(VERTICALDOOR)] = std::pair<sf::IntRect,sf::IntRect>(sf::IntRect(2*16,4*16,16,16), sf::IntRect(2*16,5*16,16,16));
	
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
	infoText.setCharacterSize(21);
	infoText.setPosition(lateralOffset/4, mapHeight + 45);
	standardHelpText = "Draw/Place Item: Left Click, Pen Size: Scroll, Fill Area: Middle Click, Box: Shift, Load: l, Save: s, Exit: ESC, Toggle Help: h";
	infoText.setString(standardHelpText);
	
	// button	
	int xPosButton = lateralOffset + mapWidth + tileOffset/2;
	int yPosButton = mapHeight - tileOffset*3;
	textB.setPos(xPosButton, yPosButton);
	textB.setSize(2*tileSize, tileSize);
	textB.setCaption("Text");
	timeB.setPos(xPosButton, yPosButton+tileOffset*1.5);
	timeB.setSize(2*tileSize, tileSize);
	timeB.setCaption("Time");
	
	// create drag tile
	mouseTile.setOutlineColor(sf::Color::Blue);
	mouseTile.setOutlineThickness(1.0f);
	mouseTile.setSize(sf::Vector2f(tileSize, tileSize));
	mouseTile.setPosition(0.5 * tileOffset, 1.0f);
	
	
	// initialize color editing with color 0 (mark in red)
	activeColorIndex = 0;
	activeItemIndex = -1;
	tileChoices[0]->setOutlineColor(sf::Color::Red);
	// set default timeout
	timeout = 20;
	timebuff = 0;
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
				// if button is clicked
				// set text button
				if (textB.isClicked(mousePos.x, mousePos.y))
				{
					if (!textB.isHighlighted())
					{
						infoText.setString("Please choose an item for which to apply a text.");
						textB.highlight();
					}
					else
					{
						infoText.setString(standardHelpText);
						textB.unhighlight();
					}
				}
				// set time button
				if (timeB.isClicked(mousePos.x, mousePos.y))
				{
					static int timeoutNow;
					if (!timeB.isHighlighted())
					{
						textOutput.setString(std::to_string(timeout));
						infoText.setString("Please choose a timeout in seconds using left and right keys and hit enter.");
						timeB.highlight();
						timeoutNow = timeout;
					}
					else
					{
						textOutput.setString("Timeout unchanged (" + std::to_string(timeoutNow) + " seconds)");
						infoText.setString(standardHelpText);
						timeB.unhighlight();
						timeout = timeoutNow;
					}
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
						if (activeItemIndex == id.at(START) || activeItemIndex == id.at(GOAL)) // portal and start item
						{
							// if item would not fit, don't place it
							if (currentY + 1 >= numTilesY)
							{
								itemTiles[currentX][currentY]->setTextureRect(tileItemRects[id.at(NOITEM)]);
							}
							else
							{
								deleteBigItem(currentX, currentY+1);
								// place big item
								if (activeItemIndex == id.at(START))
								{
									itemTiles[currentX][currentY]->setTextureRect(bigItemRects[id.at(START)].first);
									setTexture(currentX, currentY+1, bigItemRects[id.at(START)].second);
								}
								else
								{
									itemTiles[currentX][currentY]->setTextureRect(bigItemRects[id.at(GOAL)].first);
									setTexture(currentX, currentY+1, bigItemRects[id.at(GOAL)].second);
								}
							}
						}
						if (activeItemIndex == id.at(TRIGGER)) // trigger item
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
						if (activeItemIndex == id.at(DOOR)) // door item
						{
							// if door item would not fit, don't place it or place it vertically if that fits
							if (currentX + 1 >= numTilesX && currentY + 1 >= numTilesY)
							{
								itemTiles[currentX][currentY]->setTextureRect(tileItemRects[id.at(NOITEM)]);
							}
							else if (currentX + 1 >= numTilesX || itemChoices[id.at(DOOR)]->getTextureRect() == tileItemRects[id.at(VERTICALDOOR)])
							{
								deleteBigItem(currentX, currentY+1);
								itemTiles[currentX][currentY]->setTextureRect(bigItemRects[id.at(VERTICALDOOR)].first);
								setTexture(currentX, currentY+1, bigItemRects[id.at(VERTICALDOOR)].second);
							}
							else
							{
								deleteBigItem(currentX+1, currentY);
								itemTiles[currentX][currentY]->setTextureRect(bigItemRects[id.at(DOOR)].first);
								setTexture(currentX+1, currentY, bigItemRects[id.at(DOOR)].second);
							}
						}
						if (activeItemIndex >= id.at(DECO1) && activeItemIndex <= id.at(DECO4)) // deco item
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
			if (itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id.at(TRIGGER)])
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
			if (itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id.at(DECO1)]	||
				itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id.at(DECO2)]	||
				itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id.at(DECO3)]	||
				itemTiles[xPos][yPos]->getTextureRect() == tileItemRects[id.at(DECO4)]		)
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
			if (mouseTile.getTextureRect() == tileItemRects[id.at(DOOR)] || mouseTile.getTextureRect() == tileItemRects[id.at(VERTICALDOOR)])
			{
				if (itemChoices[id.at(DOOR)]->getTextureRect() == tileItemRects[id.at(VERTICALDOOR)])
				{
					itemChoices[id.at(DOOR)]->setTextureRect(tileItemRects[id.at(DOOR)]);
					mouseTile.setTextureRect(tileItemRects[id.at(DOOR)]);
				}
				else if (itemChoices[id.at(DOOR)]->getTextureRect() == tileItemRects[id.at(DOOR)])
				{
					itemChoices[id.at(DOOR)]->setTextureRect(tileItemRects[id.at(VERTICALDOOR)]);
					mouseTile.setTextureRect(tileItemRects[id.at(VERTICALDOOR)]);
				}		
			}	
		}
	}
	
	// fill area of same color with another color on middle click
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle)
	{
		int xPos = getX(window);
		int yPos = getY(window);
			
		if (activeColorIndex != -1)
		{
			floodFill(xPos, yPos, tiles[xPos][yPos]->getFillColor(), tileColors[activeColorIndex]);
		}
	}
	
	// change pensize when scrolling
	if (event.type == sf::Event::MouseWheelMoved)
	{	
		// increase pensize for drawing
		if (event.mouseWheel.delta < 0)
		{
			if (pensize < 6)
			{
				++pensize;
			}
		}
		// decrease pensize for drawing
		else
		{
			if (pensize > 1)
			{
				--pensize;
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
		if (loadLevelActive && !timeB.isHighlighted())
		{
			loadLevel(currentLevel);
			loadLevelActive = false;
			textOutput.setString("");
			infoText.setString(standardHelpText);
		}
		if (!loadLevelActive && timeB.isHighlighted())
		{
			textOutput.setString("Timeout set to " + std::to_string(timeout) + " seconds");
			infoText.setString(standardHelpText);
			timeB.unhighlight();
		}
	}
	
	// select level and timeout
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
	{
		// choose level
		if (loadLevelActive && !timeB.isHighlighted())
		{
			currentLevel = levels[nextPos(currentLevel, levels.size(), false)];
		}
		// choose timeout
		if (!loadLevelActive && timeB.isHighlighted())
		{
			// cap for 10 seconds
			if (timeout > 10) {
				timeout -= 10;
			}
			textOutput.setString(std::to_string(timeout));
		}
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
	{
		// choose level
		if (loadLevelActive && !timeB.isHighlighted())
		{
			currentLevel = levels[nextPos(currentLevel, levels.size(), true)];
		}
		// choose timeout
		if (!loadLevelActive && timeB.isHighlighted())
		{
			// cap for 500 seconds
			if (timeout < 500) {
				timeout += 10;
			}
			textOutput.setString(std::to_string(timeout));
		}
	}
	
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H)
	{
		if (infoText.getCharacterSize() == 0)
		{
			infoText.setCharacterSize(21);
		}
		else
		{
			infoText.setCharacterSize(0);
		}
	}
	
	return this;
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
					if (activeItemIndex == id.at(DOOR) && itemChoices[activeItemIndex]->getTextureRect() != tileItemRects[id.at(VERTICALDOOR)])
					{
						mouseTile.setSize(sf::Vector2f(2*tileSize + 2.0f, tileSize));
						infoText.setString("Use right click to toggle door orientation before placing.");
					}
					else if (activeItemIndex == id.at(START) || activeItemIndex == id.at(GOAL) || activeItemIndex == id.at(DOOR))
					{
						mouseTile.setSize(sf::Vector2f(tileSize, 2*tileSize + 2.0f));
						if (activeItemIndex == id.at(DOOR))
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
	// draw tile shown below the mouse if color chosen
	if (activeColorIndex != -1)
	{
		target.draw(mouseTile);
	}
	// draw itemTiles
	for (int x = 0; x < numTilesX; ++x)
	{
		for (int y = 0; y < numTilesY; ++y)
		{
			target.draw(*itemTiles[x][y]);
		}
	}
	// draw tile shown below the mouse if item chosen
	if (activeItemIndex != -1)
	{
		target.draw(mouseTile);
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
	// draw Button
	//textB.draw(target);
	timeB.draw(target);
	// draw text
	target.draw(textOutput);
	target.draw(infoText);
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
	if (itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(START)].first 		||
		itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(GOAL)].first 	||
		itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(VERTICALDOOR)].first		)
	{
		setTexture(x, y+1, tileItemRects[id.at(NOITEM)]);
	}
	if (itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(DOOR)].first)
	{
		setTexture(x+1, y, tileItemRects[id.at(NOITEM)]);
	}
	if (itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(START)].second 		||
		itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(GOAL)].second 	||
		itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(VERTICALDOOR)].second	)
	{
		setTexture(x, y-1, tileItemRects[id.at(NOITEM)]);
	}
	if (itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(DOOR)].second)
	{
		setTexture(x-1, y, tileItemRects[id.at(NOITEM)]);
	}
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
		else
		{
			std::cout << "something went wrong saving to index file" << std::endl;
			textOutput.setString("Could not save to index file.");
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
		std::cout << "something went wrong creating png file" << std::endl;
		textOutput.setString("Could not save .png file.");
		return;
	}
	
	// level name txt
	std::stringstream streamTXT;
	streamTXT << "levels/level" << currentLevel << ".txt";
	
	// save items in txt file
	std::ofstream txtfile(streamTXT.str());
	if (txtfile.is_open())
	{
		txtfile << "Timeout " << timeout << "\n";
		if (timebuff > 0) {
			txtfile << "Timebuff " << timebuff << "\n";
		}
		for (int x = 0; x < numTilesX; ++x)
		{
			for (int y = 0; y < numTilesY; ++y)
			{
				if (itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(START)].first)
				{
					txtfile << "Start " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(GOAL)].first)
				{
					txtfile << "Item Portal " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id.at(TRIGGER)])
				{
					txtfile << "Item TriggerItem " << x << " " << y << " " << triggerSwapPositionsX[x].first << " " << triggerSwapPositionsY[y].first << " " << triggerSwapPositionsX[x].second << " " << triggerSwapPositionsY[y].second << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id.at(COIN)])
				{
					txtfile << "Item CoinItem " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id.at(CLOCK)])
				{
					txtfile << "Item TimeItem " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id.at(KEY)])
				{
					txtfile << "Item KeyItem " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(DOOR)].first)
				{
					// horizontal door
					txtfile << "Item DoorItem " << x << " " << y << " 0\n";
				}
				if (itemTiles[x][y]->getTextureRect() == bigItemRects[id.at(VERTICALDOOR)].first)
				{
					// vertical door
					txtfile << "Item DoorItem " << x << " " << y << " 1\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id.at(DOORSWITCH)])
				{
					txtfile << "Item DoorSwitch " << x << " " << y << "\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id.at(DECO1)])
				{
					txtfile << "Item DecorationItem " << x << " " << y << " " << decoItemBlocking[Key(x,y)] << " 16 80 16 16\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id.at(DECO2)])
				{
					txtfile << "Item DecorationItem " << x << " " << y << " " << decoItemBlocking[Key(x,y)] << " 16 96 16 16\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id.at(DECO3)])
				{
					txtfile << "Item DecorationItem " << x << " " << y << " " << decoItemBlocking[Key(x,y)] << " 0 112 16 16\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id.at(DECO4)])
				{
					txtfile << "Item DecorationItem " << x << " " << y << " " << decoItemBlocking[Key(x,y)] << " 16 112 16 16\n";
				}
				if (itemTiles[x][y]->getTextureRect() == tileItemRects[id.at(SLIME)])
				{
					txtfile << "NPC " << x << " " << y << "\n";
				}
			}
		}
		// write text
		for (auto& element : texts)
		{
			while (!element.second.empty())
			{
				txtfile << "Text " << element.first << " " << std::get<0>(element.second.front()) << " " << std::get<1>(element.second.front()) << " " << std::get<2>(element.second.front()) << " " << std::get<3>(element.second.front()) << "\n" << std::get<4>(element.second.front()) << "\n";
				std::cout << element.first << std::endl;
				element.second.pop();
			}
		}
	}
	else
	{
		std::cout << "something went wrong creating txt file" << std::endl;
		textOutput.setString("Could not save .txt file.");
		return;
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
			itemTiles[x][y]->setTextureRect(tileItemRects[id.at(NOITEM)]);
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
			setTexture(x, y, bigItemRects[id.at(START)].first);
			setTexture(x, y+1, bigItemRects[id.at(START)].second);
		}
		if (first == "NPC")
		{
			int x, y;
			iss >> x;
			iss >> y;
			setTexture(x, y, tileItemRects[id.at(SLIME)]);
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
				setTexture(x, y, tileItemRects[id.at(COIN)]);
			}
			if (second == "TimeItem")
			{
				setTexture(x, y, tileItemRects[id.at(CLOCK)]);
			}
			if (second == "KeyItem")
			{
				setTexture(x, y, tileItemRects[id.at(KEY)]);
			}
			if (second == "DoorItem")
			{
				// TODO do sth with closed?
				int vertical;
				iss >> vertical;
				if (vertical == 1)
				{
					setTexture(x, y, bigItemRects[id.at(VERTICALDOOR)].first);
					setTexture(x, y+1, bigItemRects[id.at(VERTICALDOOR)].second);
				}
				else
				{
					setTexture(x, y, bigItemRects[id.at(DOOR)].first);
					setTexture(x+1, y, bigItemRects[id.at(DOOR)].second);
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
					setTexture(x, y, tileItemRects[id.at(DECO1)]);
				}
				// flower
				if (texPosX == 16 && texPosY == 96)
				{
					decoItemBlocking[Key(x,y)] = blocksPath;
					setTexture(x, y, tileItemRects[id.at(DECO2)]);
				}
				// crystals
				if (texPosX == 0 && texPosY == 112)
				{
					decoItemBlocking[Key(x,y)] = blocksPath;
					setTexture(x, y, tileItemRects[id.at(DECO3)]);
				}
				// rock
				if (texPosX == 16 && texPosY == 112)
				{
					decoItemBlocking[Key(x,y)] = blocksPath;
					setTexture(x, y, tileItemRects[id.at(DECO4)]);
				}
			}
			if (second == "Portal")
			{
				setTexture(x, y, bigItemRects[id.at(GOAL)].first);
				setTexture(x, y+1, bigItemRects[id.at(GOAL)].second);
			}
			if (second == "TriggerItem")
			{
				int x1, x2, y1, y2;
				iss >> x1;
				iss >> y1;
				iss >> x2;
				iss >> y2;
				setTexture(x, y, tileItemRects[id.at(TRIGGER)]);
				triggerSwapPositionsX[x] = std::pair<int, int>(x1,x2);
				triggerSwapPositionsY[y] = std::pair<int, int>(y1,y2);
			}
			if (second == "DoorSwitch")
			{
				setTexture(x, y, tileItemRects[id.at(DOORSWITCH)]);
			}
		}
		if (first == "Timeout")
		{
			iss >> timeout;
		}
		if (first == "Timebuff")
		{
			iss >> timebuff;
		}
		if (first == "Text")
		{
			std::string key = "";
			int bold;
			int r;
			int g;
			int b;
			std::string text = "";
			iss >> key >> bold >> r >> g >> b;
			std::getline(infile, line);
			text = line;
			texts[key].emplace(std::tuple<int, int, int, int, std::string>(bold, r, g, b, text));
			//event: key, coin, loose, end, start, smalltime
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

// Floodfill Algorithmus zum Austauschen von Farben aus Wikipedia
void Editor::floodFill(int x, int y, sf::Color oldColor, sf::Color newColor)
{
	if (tiles[x][y]->getFillColor() == oldColor) {
		
		tiles[x][y]->setFillColor(newColor);
		
		if (y+1 < numTilesY)
		{
			floodFill(x, y + 1, oldColor, newColor);	// unten
		}
		if (y-1 >= 0)
		{
			floodFill(x, y - 1, oldColor, newColor);	// oben
		}
		if (x-1 >= 0)
		{
			floodFill(x - 1, y, oldColor, newColor);	// links 
		}
		if (x+1 < numTilesX)
		{
			floodFill(x + 1, y, oldColor, newColor);	// rechts
		}
	}
	return;
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
