#pragma once
#include <map>
#include "Scene.hpp"

class Editor: public Scene
{
public:
	enum TileType
	{
		BLANK, GRASS, EARTH, STONEPATH, WETSTONEPATH, STONEWALL, WATER, BUSH
	};
	enum ActionItemTypes
	{
		START, GOALPORTAL, TRIGGER, COIN, CLOCK, KEY
	};
private:
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
	// offset and spaces from left to right:
	// half a tile space, tile choices, a tile space, map, same reversed ...
	// offset for the gameboard
	static const int lateralOffset = 2.5f * tileOffset;
	// gameboard tiles and itemTiles arrays (tiles are colored, itemTiles contain item textures)
	std::array<std::array<sf::RectangleShape*, numTilesY>, numTilesX> tiles;
	std::array<std::array<sf::RectangleShape*, numTilesY>, numTilesX> itemTiles;
	// bar tiles arrays (for color and item choice)
	std::array<sf::RectangleShape*, 7> tileChoices; //TODO 7 = number enums
	std::array<sf::RectangleShape*, 7> itemChoices;
	// active bar choice (color currently chosen or item currently chosen)
	int activeColorIndex;
	int activeItemIndex;
	// item texture
	const sf::Texture& actionItemTexture;
	// event active booleans
	bool mousePressed;
	bool shiftActive;
	bool triggerMarkingActive;
	std::pair<int, int> triggerMarked;
	// trigger swap positions map
	//std::map<sf::Vector2f, std::pair<sf::Vector2f, sf::Vector2f>> triggerSwapPositions;
	std::map<int, std::pair<int, int>> triggerSwapPositionsX;
	std::map<int, std::pair<int, int>> triggerSwapPositionsY;
	sf::Vector2f triggerPos;
	// mark area (e.g. for trigger)
	void markArea(int xPos, int yPos, sf::Color color, int quadrantSize);
	sf::Vector2f getMouseWorldPos(sf::RenderWindow& window);
public:
	Editor();
	~Editor();
	Scene* processEvent(sf::Event event, sf::RenderWindow& window) override final;
	Scene* update(sf::Time deltaT, sf::RenderWindow& window) override final;
	void draw(sf::RenderTarget& target, bool focus) override final;
	void saveLevel();
};
