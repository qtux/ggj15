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
private:
	std::map<int, sf::Color> _tileTypeBinding;
	// TODO get from outside
	static const int numTilesX = 30;
	static const int numTilesY = 24;
	static const int tileOffset = 32;
	static const int tileSize = 30;
	static const int quadrantSize = 6;
	static const int mapWidth = numTilesX * tileOffset;
	static const int mapHeight = numTilesY * tileOffset;
	// half a tile space, tile choices, a tile space, map, same reversed ...
	static const int lateralOffset = 2.5f * tileOffset;
	std::array<std::array<sf::RectangleShape*, numTilesY>, numTilesX> tiles;
	std::array<sf::RectangleShape*, 7> tileChoices; //TODO 7 = number enums
	std::array<sf::RectangleShape*, 7> itemChoices;
	sf::Vector2i isIn(const int xPos, const int yPos, int x1, int y1, int x2, int y2);
	int isInX(const int xPos, int x1, int x2);
	int isInY(const int yPos, int y1, int y2);
	int activeColorIndex;
	int activeItemIndex;
	bool mousePressed;
	bool shiftActive;
	bool triggerMarkingActive;
public:
	Editor();
	~Editor();
	Scene* processEvent(sf::Event event, sf::RenderWindow& window) override final;
	void update(sf::Time deltaT, sf::RenderWindow& window) override final;
	void draw(sf::RenderTarget& target) override final;
};
