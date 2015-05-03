#pragma once
#include <map>
#include <stack>
#include "Scene.hpp"

class Editor: public Scene
{
private:
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
	std::array<sf::RectangleShape*, 8> tileChoices; //TODO 7 = number enums
	std::array<sf::RectangleShape*, 12> itemChoices;
	// active bar choice (color currently chosen or item currently chosen)
	int activeColorIndex;
	int activeItemIndex;
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
	// trigger swap positions map
	//std::map<sf::Vector2f, std::pair<sf::Vector2f, sf::Vector2f>> triggerSwapPositions;
	std::map<int, std::pair<int, int>> triggerSwapPositionsX;
	std::map<int, std::pair<int, int>> triggerSwapPositionsY;
	sf::Vector2f triggerPos;
	// deco item blocking map
	std::map<Key, int> decoItemBlocking;
	// mark area (e.g. for trigger)
	void markArea(int xPos, int yPos, sf::Color color, int quadrantSize);
	// reset map when triggers were activated
	void resetTriggers();
	// text
	sf::Text textOutput;
	sf::Font font;
	// a list of the existing levels
	std::vector<int> levels;
	// the current level if loaded or was saved once
	int currentLevel;
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
