/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#pragma once
#include <map>
#include <stack>
#include <queue>
#include <string>
#include "Scene.hpp"
#include "global.hpp"

class Editor: public Scene
{
private:
	enum ActionItemType
	{
		NOITEM, START, GOAL, TRIGGER, COIN, CLOCK, KEY, DOOR, DOORSWITCH, SLIME, VERTICALDOOR, DECO1, DECO2, DECO3, DECO4
	};
	struct Key
	{
		Key() = delete;
		Key(unsigned int x, unsigned int y):
			x(x), y(y)
		{}
		unsigned int x;
		unsigned int y;
		bool operator<(const Key& rhs) const
		{
			// compare x then compare y
			return std::tie(x, y) < std::tie(rhs.x, rhs.y);
		}
	};
	struct Button
	{
		float x;
		float y;
		float width;
		float height;
		std::string caption;
		Button():
			x(0), y(0), width(0), height(0), caption("")
		{
			buttonOutside.setOutlineColor(sf::Color(90, 90, 90));
			buttonOutside.setOutlineThickness(2.0f);
			buttonOutside.setFillColor(sf::Color(122, 122, 122));
			buttonOutside.setPosition(x, y);
			buttonOutside.setSize(sf::Vector2f(0, 0));
			buttonInside.setOutlineColor(sf::Color(145, 145, 145));
			buttonInside.setOutlineThickness(1.0f);
			buttonInside.setPosition(x + 0.3, y + 0.3);
			buttonInside.setSize(sf::Vector2f(0, 0));
			buttonText.setFont(gb::ressourceManager.getFont("./fonts/LiberationSerif-Regular.ttf"));
			buttonText.setFillColor(sf::Color::Black);
			buttonText.setCharacterSize(26);
			buttonText.setPosition(x + 2, y);
			buttonText.setString(caption);
		};
		Button(float x, float y, float width, float height, std::string caption, sf::Font font):
			x(x), y(y), width(width), height(height), caption(caption)
		{
			buttonOutside.setOutlineColor(sf::Color(90, 90, 90));
			buttonOutside.setOutlineThickness(2.0f);
			buttonOutside.setFillColor(sf::Color(122, 122, 122));
			buttonOutside.setPosition(x, y);
			buttonOutside.setSize(sf::Vector2f(width, height));
			buttonInside.setOutlineColor(sf::Color(145, 145, 145));
			buttonInside.setOutlineThickness(1.0f);
			buttonInside.setPosition(x, y);
			buttonInside.setSize(sf::Vector2f(width, height));
			buttonText.setFont(font);
			buttonText.setFillColor(sf::Color::Black);
			buttonText.setCharacterSize(26);
			buttonText.setPosition(x + 2, y);
			buttonText.setString(caption);
		}
		void draw(sf::RenderTarget& target)
		{
			target.draw(buttonOutside);
			target.draw(buttonInside);
			target.draw(buttonText);
		}
		bool isClicked(float mousePosX, float mousePosY)
		{
			return mousePosX >= x && mousePosX < x + width && mousePosY >= y && mousePosY < y + height;
		}
		void highlight()
		{
			buttonOutside.setFillColor(sf::Color(95, 95, 95));
			buttonInside.setOutlineColor(sf::Color(122, 122, 122));
			buttonInside.setFillColor(sf::Color(170, 170, 170));
		}
		void unhighlight()
		{
			buttonOutside.setFillColor(sf::Color(122, 122, 122));
			buttonInside.setOutlineColor(sf::Color(145, 145, 145));
			buttonInside.setFillColor(sf::Color::White);
		}
		bool isHighlighted()
		{
			if (buttonInside.getFillColor() != sf::Color::White) {
				return true;
			}
			return false;
		}
		void setPos(float x, float y)
		{
			this->x = x;
			this->y = y;
			buttonOutside.setPosition(x, y);
			buttonInside.setPosition(x + 0.5, y + 0.5);
		}
		void setSize(float width, float height)
		{
			this->width = width;
			this->height = height;
			buttonOutside.setSize(sf::Vector2f(width, height));
			buttonInside.setSize(sf::Vector2f(width-2*1, height-2*1));
		}
		void setCaption(std::string caption)
		{
			this->caption = caption;
			buttonText.setPosition(x + 2, y);
			buttonText.setString(caption);
		}
		private:
			sf::RectangleShape buttonOutside;
			sf::RectangleShape buttonInside;
			sf::Text buttonText;
	};
	// TODO get from outside
	static const int numTilesX = 30;
	static const int numTilesY = 24;
	static const int tileOffset = 32;
	static const int tileSize = 30;
	static const int quadrantSize = 6;
	static const int mapWidth = numTilesX * tileOffset;
	static const int mapHeight = numTilesY * tileOffset;
	// maps to access color and item bar content
	std::map<int, sf::Color> tileColors;
	std::map<int, sf::IntRect> tileItemRects;
	sf::IntRect verticalDoorItemRect;
	// offset and spaces from left to right:
	// half a tile space, tile choices, a tile space, map, same reversed ...
	// offset for the gameboard
	static const int lateralOffset = 2.5f * tileOffset;
	// gameboard tiles and itemTiles arrays (tiles are colored, itemTiles contain item textures)
	std::array<std::array<sf::RectangleShape*, numTilesY>, numTilesX> tiles;
	std::array<std::array<sf::RectangleShape*, numTilesY>, numTilesX> itemTiles;
	// bar tiles arrays (for color and item choice)
	std::array<sf::RectangleShape*, 8> tileChoices; //TODO 7 = number enums
	std::array<sf::RectangleShape*, 13> itemChoices;
	// active bar choice (color currently chosen or item currently chosen)
	int activeColorIndex;
	int activeItemIndex;
	// pen size when drawing
	signed short int pensize;
	// item texture
	const sf::Texture& actionItemTexture;
	// event active booleans
	bool mousePressed;
	bool shiftActive;
	bool triggerMarkingActive;
	bool loadLevelActive;
	bool overwrite;
	bool exit;
	std::pair<int, int> triggerMarked;
	std::stack<std::pair<int, int>> triggerStack;
	// to drag
	sf::RectangleShape mouseTile;
	// trigger swap positions map
	//std::map<sf::Vector2f, std::pair<sf::Vector2f, sf::Vector2f>> triggerSwapPositions;
	std::map<int, std::pair<int, int>> triggerSwapPositionsX;
	std::map<int, std::pair<int, int>> triggerSwapPositionsY;
	sf::Vector2f triggerPos;
	// deco item blocking map
	std::map<Key, int> decoItemBlocking;
	// text
	sf::Text textOutput;
	sf::Text infoText;
	std::string standardHelpText;
	// button
	Button textB; //TODO rename to textButton
	Button timeB;
	// a list of the existing levels
	std::vector<int> levels;
	// the current level if loaded or was saved once
	int currentLevel;
	// item indices
	const std::map<ActionItemType, int> id;
	// big item IntRects
	std::map<int, std::pair<sf::IntRect, sf::IntRect>> bigItemRects;
	// timeout value
	int timeout;
	// mysterious timebuff variable (not used, just for load and save to prevent deleting it from levels)
	int timebuff;
	// list of all texts
	std::map<std::string, std::queue<std::tuple<int, int, int, int, std::string>>> texts;
	
	// mark area (e.g. for trigger)
	void markArea(int xPos, int yPos, sf::Color color, int quadrantSize);
	// reset map when triggers were activated
	void resetTriggers();
	// set texture rect
	void setTexture(int x, int y, sf::IntRect textureRect);
	// check for big item and delete if item is placed on one
	void deleteBigItem(int x, int y);
	// get x and y position of mouse in gameboard
	int getX(sf::RenderWindow& window);
	int getY(sf::RenderWindow& window);
	// check whether mouse is in gameboard
	bool mouseInGameboard(sf::RenderWindow& window);
	// floodfill algorithm
	void floodFill(int x, int y, sf::Color oldColor, sf::Color newColor);
	// get mousePos in world
	sf::Vector2f getMouseWorldPos(sf::RenderWindow& window);
public:
	Editor();
	~Editor();
	Scene* processEvent(sf::Event event, sf::RenderWindow& window) override final;
	Scene* update(sf::Time deltaT, sf::RenderWindow& window) override final;
	void draw(sf::RenderTarget& target, bool focus) override final;
	void saveLevel(bool overwrite);
	void loadLevel(int level);
	int nextPos(int pos, int size, bool clockWise);
};
