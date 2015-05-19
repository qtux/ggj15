/*
 * Level.cpp
 *
 *  Created on: 24.01.2015
 *      Author: sartz
 */

#include "Level.hpp"
#include "global.hpp"
#include <math.h>
#include "Menu.hpp"
#include "Items/KeyItem.hpp"
#include "Items/DoorItem.hpp"
#include "Highscore.hpp"
#include "Item.hpp"
#include "Player.hpp"
#include "TileMap.hpp"

// parser
#include <fstream>
#include <sstream>
#include "Items/TimeItem.hpp"
#include "Items/CoinItem.hpp"
#include "Items/TriggerItem.hpp"
#include "Items/PortalItem.hpp"
#include "Items/KeyItem.hpp"
#include "Items/DoorItem.hpp"
#include "Items/DoorSwitchItem.hpp"
#include "Items/DecorationItem.hpp"

// shader
#include <iostream>
//#include "NPC.hpp"


Level::Level(unsigned int levelNumber):
	Scene({gb::sizeX * gb::largeTileSizeX * gb::pixelSizeX, gb::sizeY * gb::largeTileSizeY * gb::pixelSizeY}),
	levelNumber(levelNumber),
	restarts(-1),
	_state(GAME)
{
	_outline.setFillColor(sf::Color(0x00, 0x00, 0x00, 0x00));
	_outline.setOutlineColor(sf::Color(0x90, 0x90, 0x00));
	_outline.setOutlineThickness(2.0f);
	_outline.setPosition(0, 0);
	_outline.setSize(sceneSize);
	background.setTexture(&gb::textureManager.getTexture("img/background.png", true));
	
	reset();
	// load only the fragment shader
	_fragmentShader.loadFromFile("src/shader/fragment_shader.frag", sf::Shader::Fragment);
	if (!_fragmentShader.isAvailable())
	{
	    std::cerr<<"could not load shader"<<std::endl;
	}
}

sf::Uint32 Level::createColorKey(sf::Color color) {
	
	sf::Uint32 colorKey = 0;
	
	colorKey |= color.r << 3*8;
	colorKey |= color.g << 2*8;
	colorKey |= color.b << 1*8;
	colorKey |= color.a << 0*8;
	
	return colorKey;
}

void Level::reset()
{
	restarts++;
	_state = GAME;
	
	// TODO implement reset of the level (new init) --> contains a bug that adds items to the list everytime the level will be restarted
	textBox = new TextBox();
	highscore = new Highscore(levelNumber, sf::Vector2f(gb::gridWidth, gb::gridHeight));
	
	// load and set timebar
	gui = new GUI(this);
	gui->setTimeout(20);
	// load image bitmapt file
	sf::Image levelImg;
	
	// try to load the file
	std::string fileName = std::string(PATH) + "levels/level" + std::to_string(levelNumber);
	if (!levelImg.loadFromFile(fileName+".png")) {
		// TODO return to menu here
		return;
	}
	
	// create a tileset
	std::map<sf::Uint32, bool> colorToSolid;
	colorToSolid[0x000100ff] = true;	// wall
	colorToSolid[0x5f5f5fff] = false;	// wet stone
	colorToSolid[0x9b6d27ff] = false;	// dirt
	colorToSolid[0x969896ff] = false;	// stone
	colorToSolid[0x11941bff] = false;	// grass
	colorToSolid[0x003E04ff] = true;	// trees
	colorToSolid[0x0000abff] = true;	// water
	
	std::map<sf::Uint32, unsigned int> colorToInt;
	colorToInt[0x000100ff] = 6;	// wall
	colorToInt[0x5f5f5fff] = 4;	// wet stone
	colorToInt[0x9b6d27ff] = 3;	// dirt
	colorToInt[0x969896ff] = 2;	// stone
	colorToInt[0x11941bff] = 1;	// grass
	colorToInt[0x003E04ff] = 5;	// trees
	colorToInt[0x0000abff] = 0;	// water
	
	const sf::Vector2u gridSize(gb::sizeX * gb::largeTileSizeX, gb::sizeY * gb::largeTileSizeY);
	// TODO determine the following two variables in another way
	const sf::Vector2u tileSize(gb::pixelSizeX, gb::pixelSizeY);
	const sf::Vector2f scale(0.5f, 0.5f);
	
	std::vector<unsigned int> mapping;
	std::vector<bool> collision;
	for (int x = 0; x < gridSize.y; ++x)
	{
		for (int y = 0; y < gridSize.x; ++y)
		{
			sf::Uint32 colorKey = createColorKey(levelImg.getPixel(y, x));
			mapping.push_back(colorToInt[colorKey]);
			collision.push_back(colorToSolid[colorKey]);
		}
	}
	
	const sf::Texture& baseTileSet = gb::textureManager.getTexture(std::string(PATH) + "img/tileset.png", false);
	sf::Vector2f offset(-6, -6);
	const sf::Texture& tileSet = gb::textureManager.getTileSet(baseTileSet, mapping, tileSize, gridSize, offset);
	map = new TileMap(tileSize, {gb::largeTileSizeX * gb::sizeX, gb::largeTileSizeY * gb::sizeY}, tileSet, collision);
	
	// read text file
	std::ifstream infile(fileName + ".txt");
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string first;
		iss >> first;
		// TODO do a sanity check on items placed at the same coordinate
		if (first == "Start")
		{
			unsigned int x,y;
			iss >> x >> y;
			// TODO remove static_casts
			player = new Player(
				{static_cast<float>(x * gb::pixelSizeX), static_cast<float>(y * gb::pixelSizeY)},
				{static_cast<float>(gb::pixelSizeX), static_cast<float>(2 * gb::pixelSizeY)},
				{static_cast<float>(gb::pixelSizeX), static_cast<float>(gb::pixelSizeY)}
			);
		}
		if (first == "Portal")
		{
			unsigned int x,y;
			iss >> x >> y;
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/items.png", false));
			//sprite->setPosition(10, gb::gridHeight - 30);
			Item *tmpItem = new PortalItem(sprite);
			tmpItem->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			items[Key(x, y)] = tmpItem;
		}
		/*if (first == "NPC")
		{ //TODO vorläufige Version, sollte verschönert werden
			//std::string second;
			//iss >> second;
			int x,y;
			iss >> x;
			iss >> y;

			NPC * tmpNPC = new NPC(this);
			sf::Sprite *npcSprite = new sf::Sprite();
			npcSprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/npc.png", false));
			tmpNPC->mySprite = npcSprite;
			tmpNPC->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			//std::cout<<"NPC loaded"<<std::endl;
			npcs.push_back(tmpNPC);
		}*/
		if (first == "Item")
		{
			std::string second;
			unsigned int x,y;
			iss >> second >> x >> y;
			Item* tmpItem = nullptr;
			if (second == "DecorationItem")
			{
				bool blocksPath;
				int texPosX, texPosY, texW, texH;
				iss >> blocksPath >> texPosX >> texPosY >> texW >> texH;
				sf::Sprite* sprite = new sf::Sprite();
				sprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/items.png", false));
				//sprite->setPosition(10, gb::gridHeight - 30);
				tmpItem = new DecorationItem(sprite, blocksPath, texPosX, texPosY, texW, texH);
			}
			else if (second == "DoorItem")
			{
				bool vertical = false;
				bool closed = true;
				iss >> vertical >> closed;
				sf::Sprite* sprite = new sf::Sprite();
				sprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/items.png", false));
				//sprite->setPosition(10, gb::gridHeight - 30);
				tmpItem = new DoorItem(sprite, vertical, true);
			}
			else if (second == "TimeItem")
			{
				sf::Sprite* sprite = new sf::Sprite();
				sprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/items.png", false));
				//sprite->setPosition(10, gb::gridHeight - 30);
				tmpItem = new TimeItem(sprite);
			}
			else if (second == "CoinItem")
			{
				sf::Sprite* sprite = new sf::Sprite();
				sprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/items.png", false));
				//sprite->setPosition(10, gb::gridHeight - 30);
				tmpItem = new CoinItem(sprite);
			}
			else if (second == "KeyItem")
			{
				sf::Sprite* sprite = new sf::Sprite();
				sprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/items.png", false));
				//sprite->setPosition(10, gb::gridHeight - 30);
				tmpItem = new KeyItem(sprite);
			}
			
			if (tmpItem != nullptr)
			{
				tmpItem->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
				items[Key(x, y)] = tmpItem;
			}
		}
		
		if (first == "DoorSwitch")
		{
			unsigned int x,y;
			iss >> x >> y;
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/items.png", false));
			//sprite->setPosition(10, gb::gridHeight - 30);
			Item *tmpItem = new DoorSwitchItem(sprite, false);
			tmpItem->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			items[Key(x, y)] = tmpItem;
		}
		
		if (first == "Timeout")
		{
			int time;
			iss >> time;
			gui->setTimeout(time);
		}
		if (first == "Timebuff")
		{
			int time;
			iss >> time;
			gui->setTimebuffFactor(time);
		}
		if (first == "Text")
		{
			TextBox::TextElement* element = new TextBox::TextElement();
			std::string boldText = "";
			iss >> element->eventType >> boldText >> element->r >> element->g >> element->b;
			std::getline(infile, line);
			element->text = line;
			if (boldText=="bold")
			{
				element->bold = true;
			}
			textBox->appendText(element);
		}
		if (first == "TriggerItem")
		{
			unsigned int x, y, x1, x2, y1, y2;
			iss >> x >> y >> x1 >> y1 >> x2 >> y2;
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/items.png", false));
			//sprite->setPosition(10, gb::gridHeight - 30);
			TriggerItem *tmpItem = new TriggerItem(sprite);
			tmpItem->setSwitchPos(x1, y1, x2, y2);
			tmpItem->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			items[Key(x, y)] = tmpItem;
		}
		if (first == "TriggerTrapItem")
		{
			unsigned int x, y, x1, x2, y1, y2;
			iss >> x >> y >> x1 >> y1 >> x2 >> y2;
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(gb::textureManager.getTexture(std::string(PATH) + "img/items.png", false));
			//sprite->setPosition(10, gb::gridHeight - 30);
			sprite->setTextureRect(sf::IntRect(3 * 16, 6 * 16, 16, 16));
			TriggerItem *tmpItem = new TriggerItem(sprite);
			tmpItem->setSwitchPos(x1, y1, x2, y2);
			tmpItem->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			items[Key(x, y)] = tmpItem;
		}
	}
	
	// trigger text
	textBox->triggerText("start");
}

Scene* Level::processEvent(sf::Event event, sf::RenderWindow& window)
{
	// preprocessing key input (to enhance code readability)
	sf::Keyboard::Key keyPressed = sf::Keyboard::Unknown;
	if (event.type == sf::Event::KeyPressed)
	{
		keyPressed = event.key.code;
	}
	
	// process events
	if (keyPressed == sf::Keyboard::Escape)
	{
		return new Menu(Menu::Command::LEVEL);
	}
	if (keyPressed == sf::Keyboard::Space)
	{
		if (_state == HIGHSCORE)
		{
			return new Level(levelNumber + 1);
		}
		else
		{
			textBox->pushText();
		}
	}
	if (keyPressed == sf::Keyboard::S)
	{
		if (_state != HIGHSCORE)
		{
			textBox->skip();
		}
	}
	
	return this;
}

Scene* Level::update(sf::Time deltaT, sf::RenderWindow& window)
{
	// load highscore once
	if (_state == LEAVING && !textBox->enabled())
	{
		highscore->save(gui->coins, gui->timeLeft(), gui->timeoutSeconds, restarts);
		highscore->load();
		_state = HIGHSCORE;
	}
	
	// update game logic while in GAME state
	if (_state == GAME && !textBox->enabled())
	{
		// determine the movement vector based on input and move the player appropriately
		sf::Vector2f moveDir(0, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			moveDir.x -= 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			moveDir.x += 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			moveDir.y -= 1;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			moveDir.y += 1;
		}
		player->move(deltaT, moveDir, sceneSize, map);
		
		// update npcs
		/*for (auto& obj : npcs)
		{
			obj->update(deltaT);
		}*/
		
		// check if the player activated an item while moving on it
		// TODO get items situated on the same place as the player and iterate only over them
		for (auto& kv: items)
		{
			const sf::Vector2i tilePos(kv.first.x, kv.first.y);
			const sf::Vector2i tileSize(gb::pixelSizeX, gb::pixelSizeY);
			if (player->intersects(tilePos, tileSize))
			{
				kv.second->applyEffect(*this);
				if (kv.second->collectable)
				{
					items.erase(kv.first);
				}
			}
		}
	}
	
	// update map, gui and items (mostly animation state) while not in HIGHSCORE state
	if (_state != HIGHSCORE)
	{
		map->update(deltaT);
		gui->update(deltaT);
		for(auto& kv: items)
		{
			kv.second->update(deltaT);
		}
		// update doggie and playeranimation
		player->update(deltaT);
	}
	
	return this;
}

void Level::draw(sf::RenderTarget &renderTarget, bool focus)
{
	renderTarget.draw(background);
	renderTarget.draw(_outline);
	map->draw(renderTarget);
	for(auto& kv: items)
	{
		kv.second->draw(renderTarget, nullptr);
	}
	/*for (auto& obj: npcs) {
		obj->draw(renderTarget, nullptr);
	}*/
	
	player->draw(renderTarget, nullptr);
	gui->draw(renderTarget);
	textBox->draw(renderTarget);
	if (_state == HIGHSCORE)
	{
		highscore->draw(renderTarget);
	}
}

bool Level::readyToLeave() const
{
	int keysInLevel = 0;
	for(auto& kv: items)
	{
		if (dynamic_cast<KeyItem*>(kv.second))
		{
			keysInLevel++;
		}
	}
	return (keysInLevel == 0);
}

void Level::leave()
{
	if (!readyToLeave())
	{
		return;
	}
	_state = LEAVING;
	textBox->triggerText("end");
}

// TODO remove this and register switchRange in a scripting language
void Level::switchLargeTile(const sf::Vector2u& first, const sf::Vector2u& second)
{
	map->switchRange(first, second, {gb::largeTileSizeX, gb::largeTileSizeY}, sf::seconds(0.5f));
}

void Level::toggleDoors()
{
	for(auto& kv: items)
	{
		if (dynamic_cast<DoorItem*>(kv.second))
		{
			dynamic_cast<DoorItem*>(kv.second)->toggle();
		}
	}
}
