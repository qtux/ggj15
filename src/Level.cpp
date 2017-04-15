/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "Level.hpp"
#include "global.hpp"
#include "Menu.hpp"
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
	background.setTexture(&gb::ressourceManager.getTexture("img/background.png", true));
	
	reset();
	// load only the fragment shader
	_fragmentShader.loadFromFile("src/shader/fragment_shader.frag", sf::Shader::Fragment);
	if (!_fragmentShader.isAvailable())
	{
	    std::cerr<<"could not load shader"<<std::endl;
	}
}

bool Level::parseLevel(std::string fileName)
{
	// check text file
	std::ifstream infile(fileName + ".txt");
	if (!infile.good())
	{
		return false;
	}
	
	// read text file
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
		/*if (first == "NPC")
		{ //TODO vorläufige Version, sollte verschönert werden
			//std::string second;
			//iss >> second;
			int x,y;
			iss >> x;
			iss >> y;

			NPC * tmpNPC = new NPC(this);
			sf::Sprite *npcSprite = new sf::Sprite();
			npcSprite->setTexture(gb::ressourceManager.getTexture(std::string(PATH) + "img/npc.png", false));
			tmpNPC->mySprite = npcSprite;
			tmpNPC->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			//std::cout<<"NPC loaded"<<std::endl;
			npcs.push_back(tmpNPC);
		}*/
		if (first == "Item")
		{
			std::string itemType;
			unsigned int x,y;
			iss >> itemType >> x >> y;
			Item* tmpItem = nullptr;
			sf::Sprite* sprite = new sf::Sprite();
			sprite->setTexture(gb::ressourceManager.getTexture(std::string(PATH) + "img/items.png", false));
			sprite->setPosition(x * gb::pixelSizeX, y * gb::pixelSizeY);
			
			if (itemType == "DecorationItem")
			{
				bool blocksPath;
				std::string type;
				iss >> blocksPath >> type;
				tmpItem = new DecorationItem(sprite, blocksPath, type);
			}
			else if (itemType == "DoorItem")
			{
				bool vertical = false;
				bool closed = true;
				iss >> vertical >> closed;
				tmpItem = new DoorItem(sprite, vertical, true);
			}
			else if (itemType == "TimeItem")
			{
				tmpItem = new TimeItem(sprite);
			}
			else if (itemType == "CoinItem")
			{
				tmpItem = new CoinItem(sprite);
			}
			else if (itemType == "KeyItem")
			{
				tmpItem = new KeyItem(sprite);
			}
			else if (itemType == "DoorSwitch")
			{
				tmpItem = new DoorSwitchItem(sprite, false);
			}
			else if (itemType == "TriggerItem")
			{
				unsigned int x1, x2, y1, y2;
				iss >> x1 >> y1 >> x2 >> y2;
				tmpItem = new TriggerItem(sprite, {x1, y1}, {x2, y2});
			}
			else if (itemType == "TriggerTrapItem")
			{
				unsigned int x1, x2, y1, y2;
				iss >> x1 >> y1 >> x2 >> y2;
				sprite->setTextureRect(sf::IntRect(3 * 16, 6 * 16, 16, 16));
				tmpItem = new TriggerItem(sprite, {x1, y1}, {x2, y2});
			}
			else if (itemType == "Portal")
			{
				tmpItem = new PortalItem(sprite);
			}
			
			if (tmpItem != nullptr)
			{
				items[Key(x, y)] = tmpItem;
			}
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
	}
	
	// load tilemap
	const sf::Vector2u gridSize(gb::sizeX * gb::largeTileSizeX, gb::sizeY * gb::largeTileSizeY);
	const sf::Vector2u tileSize(gb::pixelSizeX, gb::pixelSizeY);
	map = new TileMap(tileSize, gridSize, fileName);
	return true;
}

void Level::reset()
{
	restarts++;
	_state = GAME;
	
	textBox = new TextBox();
	highscore = new Highscore(levelNumber, sf::Vector2f(gb::gridWidth, gb::gridHeight));
	
	// load and set timebar
	gui = new GUI(this);
	gui->setTimeout(20);
	
	// try to load the file
	std::string fileName = std::string(PATH) + "levels/level" + std::to_string(levelNumber);
	if (!parseLevel(fileName))
	{
		_state = ABORT;
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
		return new Menu(Menu::Command::LEVEL, levelNumber);
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
	if (_state == ABORT) {
		return new Menu(Menu::Command::LEVEL);
	}
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
