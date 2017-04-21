/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "TileMap.hpp"
#include "global.hpp"

bool colorToSolid(sf::Uint32 color)
{
	switch (color)
	{
		case 0x000100ff: return true;	// wall
		case 0x5f5f5fff: return false;	// wet stone
		case 0x9b6d27ff: return false;	// dirt
		case 0x969896ff: return false;	// stone
		case 0x11941bff: return false;	// grass
		case 0x003E04ff: return true;	// trees
		case 0x0000abff: return true;	// water
		default: return true;			// water
	}
}

int colorToInt(sf::Uint32 color)
{
	switch (color)
	{
		case 0x000100ff: return 6;	// wall
		case 0x5f5f5fff: return 4;	// wet stone
		case 0x9b6d27ff: return 3;	// dirt
		case 0x969896ff: return 2;	// stone
		case 0x11941bff: return 1;	// grass
		case 0x003E04ff: return 5;	// trees
		case 0x0000abff: return 0;	// water
		default: return 0;			// water
	}
}

TileMap::TileMap(const sf::Vector2u& tileSize, const sf::Vector2u& gridSize, const std::string fileName):
	tileSize(tileSize),
	gridSize(gridSize),
	_vertices(sf::VertexArray(sf::Quads, gridSize.x * gridSize.y * 4))
{
	// try to load the image bitmap file
	sf::Image levelImg;
	if (!levelImg.loadFromFile(fileName + ".png")) {
		levelImg.create(gridSize.y, gridSize.x);
	}
	
	// define the _solid and _texture vectors
	for (int x = 0; x < gridSize.y; ++x)
	{
		for (int y = 0; y < gridSize.x; ++y)
		{
			sf::Color color = levelImg.getPixel(y, x);
			sf::Uint32 colorKey = 0;
			colorKey |= color.r << 3*8;
			colorKey |= color.g << 2*8;
			colorKey |= color.b << 1*8;
			colorKey |= color.a << 0*8;
			_mapping.push_back(colorToInt(colorKey));
			_solid.push_back(colorToSolid(colorKey));
		}
	}
	_baseTileSet = &gb::ressourceManager.getTexture(std::string(PATH) + "img/tileset.png", false);
	_texture = &gb::ressourceManager.getTileSet(*_baseTileSet, _mapping, tileSize, gridSize, sf::Vector2f(-6, -6));
	
	// create quadfs for every tile
	for (auto i = 0; i < gridSize.x; ++i)
	{
		for (auto j = 0; j < gridSize.y; ++j)
		{
			// get a pointer to the current quad
			sf::Vertex* quad = &_vertices[(i + j * gridSize.x) * 4];
			// define its 4 corners
			quad[0].position = sf::Vector2f(i		* tileSize.x,	j		* tileSize.y);
			quad[1].position = sf::Vector2f((i + 1)	* tileSize.x,	j		* tileSize.y);
			quad[2].position = sf::Vector2f((i + 1)	* tileSize.x,	(j + 1)	* tileSize.y);
			quad[3].position = sf::Vector2f(i		* tileSize.x,	(j + 1)	* tileSize.y);
			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(i		* tileSize.x,	j		* tileSize.y);
			quad[1].texCoords = sf::Vector2f((i + 1)* tileSize.x,	j		* tileSize.y);
			quad[2].texCoords = sf::Vector2f((i + 1)* tileSize.x,	(j + 1)	* tileSize.y);
			quad[3].texCoords = sf::Vector2f(i		* tileSize.x,	(j + 1)	* tileSize.y);
		}
	}
}

void TileMap::switchTile(const sf::Vector2u& first, const sf::Vector2u& second, const sf::Time& duration)
{
	// determine the indices in the one dimensional vertex array
	int firstIndex = first.x + gridSize.x * first.y;
	int secondIndex = second.x + gridSize.x * second.y;
	// and get pointer to both quads
	sf::Vertex* firstQuad = &_vertices[firstIndex * 4];
	sf::Vertex* secondQuad = &_vertices[secondIndex * 4];
	
	// smoothly interchange tiles: create two TileAnimation and add it to the list of animations
	_movingTiles.emplace_front(
		new sf::Sprite(
			*_texture,
			sf::IntRect(
				firstQuad[0].texCoords.x,
				firstQuad[0].texCoords.y,
				tileSize.x,
				tileSize.y
			)
		),
		firstQuad[0].position,
		secondQuad[0].position,
		duration,
		first
	);
	
	_movingTiles.emplace_front(
		new sf::Sprite(
			*_texture,
			sf::IntRect(
				secondQuad[0].texCoords.x,
				secondQuad[0].texCoords.y,
				tileSize.x,
				tileSize.y
			)
		),
		secondQuad[0].position,
		firstQuad[0].position,
		duration,
		second
	);
	
	// swap collision data and tile mapping data
	std::swap(_solid[firstIndex], _solid[secondIndex]);
	std::swap(_mapping[firstIndex], _mapping[secondIndex]);
	
	// move both quads out of view TODO to minus infinity?
	sf::Vector2f outside = sf::Vector2f(-tileSize.x, -tileSize.y);
	firstQuad[0].position = outside;
	firstQuad[1].position = outside;
	firstQuad[2].position = outside;
	firstQuad[3].position = outside;
	secondQuad[0].position = outside;
	secondQuad[1].position = outside;
	secondQuad[2].position = outside;
	secondQuad[3].position = outside;
}

void TileMap::switchRange(const sf::Vector2u& first, const sf::Vector2u& second, const sf::Vector2u& size, const sf::Time& duration)
{
	for (auto i = 0;  i < size.x; ++i)
	{
		for (auto j = 0; j < size.y; ++j)
		{
			sf::Vector2u offset(i, j);
			switchTile(first + offset, second + offset, duration);
		}
	}
}

void TileMap::update(const sf::Time& deltaT)
{
	auto lambda = [&](TileAnimation& ani){
		// update the animation
		if (ani.update(deltaT))
		{
			// if the animation finished move the corresponding quads to their original position
			unsigned int i = ani.quadIndex.x;
			unsigned int j = ani.quadIndex.y;
			sf::Vertex* quad = &_vertices[(i + j * gridSize.x) * 4];
			quad[0].position = sf::Vector2f(i		* tileSize.x,	j		* tileSize.y);
			quad[1].position = sf::Vector2f((i + 1)	* tileSize.x,	j		* tileSize.y);
			quad[2].position = sf::Vector2f((i + 1)	* tileSize.x,	(j + 1)	* tileSize.y);
			quad[3].position = sf::Vector2f(i		* tileSize.x,	(j + 1)	* tileSize.y);
			return true;
		}
		return false;
	};
	// iterate over movingTiles and remove an animation if finished
	bool wasEmpty = _movingTiles.empty();
	_movingTiles.remove_if(lambda);
	// regenerate texture if the last moving tile was removed
	if (!wasEmpty && _movingTiles.empty()) {
		_texture = &gb::ressourceManager.getTileSet(*_baseTileSet, _mapping, tileSize, gridSize, sf::Vector2f(-6, -6));
	}
}

void TileMap::draw(sf::RenderTarget& target)
{
	target.draw(_vertices, _texture);
	for (auto& ani: _movingTiles)
	{
		target.draw(*ani.sprite);
	}
}
