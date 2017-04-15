/*
 * Copyright (c) 2015-2017 Annemarie Mattmann, Johannes Mattmann,
 * Matthias Gazzari, Moritz Hagemann, Sebastian Artz
 * 
 * Licensed under the MIT license. See the LICENSE file for details.
 */

#include "RessourceManager.hpp"
#include <algorithm>

const sf::Font& RessourceManager::getFont(std::string fileName)
{
	auto search = _fonts.find(fileName);
	if (search == _fonts.end())
	{
		_fonts.emplace(fileName, sf::Font());
		_fonts[fileName].loadFromFile(fileName);
	}
	return _fonts[fileName];
}

const sf::Texture& RessourceManager::getTexture(std::string fileName, bool repeat, bool smooth)
{
	Key key = Key(fileName, repeat, smooth);
	auto search = _textures.find(key);
	if (search == _textures.end())
	{
		_textures.emplace(key, sf::Texture());
		_textures[key].loadFromFile(fileName);
		_textures[key].setRepeated(repeat);
		_textures[key].setSmooth(smooth);
	}
	return _textures[key];
}

const sf::Texture& RessourceManager::getTileSet(const sf::Texture& baseTileSet, const std::vector<unsigned int>& mapping, const sf::Vector2u& tileSize, const sf::Vector2u& gridSize, const sf::Vector2f& offset, bool smooth)
{
	// increase the size of the render texture by one in each direction
	// this is required to re-center the tiles (the tiles appear to be
	// shifted due to adding a border on the right and bottom side)
	sf::Vector2u vertexSize = gridSize + sf::Vector2u(1, 1);
	
	// create the render texture and clear it setting it to blue
	if (!renderTex.create(gridSize.x * tileSize.x, gridSize.y * tileSize.y))
	{
		return renderTex.getTexture();
	}
	renderTex.clear(sf::Color::Blue);
	// apply smooth filter if requested
	renderTex.setSmooth(smooth);
	
	// create a vertex array with one quad per tile and define edge positions
	sf::VertexArray targetVertices(sf::Quads, vertexSize.x * vertexSize.y * 4);
	for (unsigned int i = 0; i < vertexSize.x; ++i)
	{
		for (unsigned int j = 0; j < vertexSize.y; ++j)
		{
			sf::Vertex* quad = &targetVertices[(i + j * vertexSize.x) * 4];
			quad[0].position = sf::Vector2f(i		* tileSize.x,	j		* tileSize.y) + offset;
			quad[1].position = sf::Vector2f((i + 1)	* tileSize.x,	j		* tileSize.y) + offset;
			quad[2].position = sf::Vector2f((i + 1)	* tileSize.x,	(j + 1)	* tileSize.y) + offset;
			quad[3].position = sf::Vector2f(i		* tileSize.x,	(j + 1)	* tileSize.y) + offset;
		}
	}
	
	// draw basis tiles
	for (unsigned int i = 0; i < vertexSize.x; ++i)
	{
		for (unsigned int j = 0; j < vertexSize.y; ++j)
		{
			sf::Vertex* quad = &targetVertices[(i + j * vertexSize.x) * 4];
			unsigned int x = std::min(i, gridSize.x - 1);
			unsigned int y = std::min(j, gridSize.y - 1);
			// define texture coordinates
			unsigned int u = rand() % 6;						// column: random subtype
			unsigned int v = mapping[x + y * gridSize.x];		// row: type identification
			quad[0].texCoords = sf::Vector2f(u			* tileSize.x,	v		* tileSize.y);
			quad[1].texCoords = sf::Vector2f((u + 1)	* tileSize.x,	v		* tileSize.y);
			quad[2].texCoords = sf::Vector2f((u + 1)	* tileSize.x,	(v + 1)	* tileSize.y);
			quad[3].texCoords = sf::Vector2f(u			* tileSize.x,	(v + 1)	* tileSize.y);
		}
	}
	renderTex.draw(targetVertices, &baseTileSet);
	
	//draw top border tiles
	for (unsigned int i = 0; i < vertexSize.x; ++i)
	{
		for (unsigned int j = 0; j < vertexSize.y; ++j)
		{
			sf::Vertex* quad = &targetVertices[(i + j * vertexSize.x) * 4];
			sf::Uint8 alpha = 0;
			if (j > 0)
			{
				unsigned int x = std::min(i, gridSize.x - 1);
				unsigned int y = std::min(j, gridSize.y - 1);
				unsigned int yDec = std::min(j - 1, gridSize.y - 1);
				// define texture coordinates and alpha
				unsigned int v = mapping[x + yDec * gridSize.x];		// row: type identification
				alpha = (v != mapping[x + y * gridSize.x]) ? 255 : 0;
				unsigned int u = 6 + rand() % 4;						// column: random subtype
				quad[0].texCoords = sf::Vector2f(u			* tileSize.x,	v		* tileSize.y);
				quad[1].texCoords = sf::Vector2f((u + 1)	* tileSize.x,	v		* tileSize.y);
				quad[2].texCoords = sf::Vector2f((u + 1)	* tileSize.x,	(v + 1)	* tileSize.y);
				quad[3].texCoords = sf::Vector2f(u			* tileSize.x,	(v + 1)	* tileSize.y);
			}
			quad[0].color.a = alpha;
			quad[1].color.a = alpha;
			quad[2].color.a = alpha;
			quad[3].color.a = alpha;
		}
	}
	renderTex.draw(targetVertices, &baseTileSet);
	
	// draw left border tiles
	for (unsigned int i = 0; i < vertexSize.x; ++i)
	{
		for (unsigned int j = 0; j < vertexSize.y; ++j)
		{
			sf::Vertex* quad = &targetVertices[(i + j * vertexSize.x) * 4];
			sf::Uint8 alpha = 0;
			if (i > 0)
			{
				unsigned int x = std::min(i, gridSize.x - 1);
				unsigned int y = std::min(j, gridSize.y - 1);
				unsigned int xDec = std::min(i - 1, gridSize.x - 1);
				// define texture coordinates and alpha
				unsigned int v = mapping[xDec + y * gridSize.x];		// row: type identification
				alpha = (v != mapping[x + y * gridSize.x]) ? 255 : 0;
				unsigned int u = 10 + rand() % 4;						// column: random subtype
				quad[0].texCoords = sf::Vector2f(u			* tileSize.x,	v		* tileSize.y);
				quad[1].texCoords = sf::Vector2f((u + 1)	* tileSize.x,	v		* tileSize.y);
				quad[2].texCoords = sf::Vector2f((u + 1)	* tileSize.x,	(v + 1)	* tileSize.y);
				quad[3].texCoords = sf::Vector2f(u			* tileSize.x,	(v + 1)	* tileSize.y);
			}
			quad[0].color.a = alpha;
			quad[1].color.a = alpha;
			quad[2].color.a = alpha;
			quad[3].color.a = alpha;
		}
	}
	renderTex.draw(targetVertices, &baseTileSet);
	
	// draw top-left border tiles
	for (unsigned int i = 0; i < vertexSize.x; ++i)
	{
		for (unsigned int j = 0; j < vertexSize.y; ++j)
		{
			// get a pointer to the current quad
			sf::Vertex* quad = &targetVertices[(i + j * vertexSize.x) * 4];
			sf::Uint8 alpha = 0;
			if (i > 0 && j > 0)
			{
				unsigned int x = std::min(i, gridSize.x - 1);
				unsigned int y = std::min(j, gridSize.y - 1);
				unsigned int xDec = std::min(i - 1, gridSize.x - 1);
				unsigned int yDec = std::min(j - 1, gridSize.y - 1);
				// define texture coordinates and alpha
				unsigned int v = mapping[xDec + yDec * gridSize.x];		// row: type identification
				alpha = (v != mapping[x + yDec * gridSize.x] || v != mapping[xDec + y * gridSize.x]) ? 255 : 0;
				unsigned int u = 14 + rand() % 2;							// column: random subtype
				quad[0].texCoords = sf::Vector2f(u			* tileSize.x,	v		* tileSize.y);
				quad[1].texCoords = sf::Vector2f((u + 1)	* tileSize.x,	v		* tileSize.y);
				quad[2].texCoords = sf::Vector2f((u + 1)	* tileSize.x,	(v + 1)	* tileSize.y);
				quad[3].texCoords = sf::Vector2f(u			* tileSize.x,	(v + 1)	* tileSize.y);
			}
			quad[0].color.a = alpha;
			quad[1].color.a = alpha;
			quad[2].color.a = alpha;
			quad[3].color.a = alpha;
		}
	}
	renderTex.draw(targetVertices, &baseTileSet);
	
	//finish the texture
	renderTex.display();
	
	// retrieve texture and store it
	return renderTex.getTexture();
}
