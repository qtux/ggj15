#include "TextureManager.hpp"

const sf::Texture& TextureManager::getTexture(std::string fileName, bool repeat, bool smooth)
{
	// weird bug fix (search for static initialisation of a map --> segfault) ...
	if (_textures.empty())
	{
		_textures.clear();
	}
	// normal code starts here
	Key key = Key(fileName, repeat, smooth);
	auto search = _textures.find(key);
	if (search == _textures.end())
	{
		_textures[Key("a",false, false)] = sf::Texture();
		_textures.emplace(key, sf::Texture());
		_textures[key].loadFromFile(fileName);
		_textures[key].setRepeated(repeat);
		_textures[key].setSmooth(smooth);
	}
	return _textures[key];
}

const sf::Texture& TextureManager::getTileSet(const sf::Texture& baseTileSet, const std::vector<unsigned int>& mapping, const sf::Vector2u& tileSize, const sf::Vector2u& gridSize, bool smooth)
{
	// create the render texture and clear it setting it to blue
	if (!renderTex.create(gridSize.x * tileSize.x, gridSize.y * tileSize.y))
	{
		//return nullptr;
		renderTex.getTexture();
	}
	renderTex.clear(sf::Color::Blue);
	// apply smooth filter if requested
	renderTex.setSmooth(smooth);
	
	// create a vertex array with one quad per tile and define edge positions
	sf::VertexArray targetVertices(sf::Quads, gridSize.x * gridSize.y * 4);
	for (auto i = 0; i < gridSize.x; ++i)
	{
		for (auto j = 0; j < gridSize.y; ++j)
		{
			sf::Vertex* quad = &targetVertices[(i + j * gridSize.x) * 4];
			quad[0].position = sf::Vector2f(i		* tileSize.x,	j		* tileSize.y);
			quad[1].position = sf::Vector2f((i + 1)	* tileSize.x,	j		* tileSize.y);
			quad[2].position = sf::Vector2f((i + 1)	* tileSize.x,	(j + 1)	* tileSize.y);
			quad[3].position = sf::Vector2f(i		* tileSize.x,	(j + 1)	* tileSize.y);
		}
	}
	
	// draw basis tiles
	for (auto i = 0; i < gridSize.x; ++i)
	{
		for (auto j = 0; j < gridSize.y; ++j)
		{
			sf::Vertex* quad = &targetVertices[(i + j * gridSize.x) * 4];
			// define texture coordinates
			unsigned int u = rand() % 6;						// column: random subtype
			unsigned int v = mapping[i + j * gridSize.x];		// row: type identification
			quad[0].texCoords = sf::Vector2f(u			* tileSize.x,	v		* tileSize.y);
			quad[1].texCoords = sf::Vector2f((u + 1)	* tileSize.x,	v		* tileSize.y);
			quad[2].texCoords = sf::Vector2f((u + 1)	* tileSize.x,	(v + 1)	* tileSize.y);
			quad[3].texCoords = sf::Vector2f(u			* tileSize.x,	(v + 1)	* tileSize.y);
		}
	}
	renderTex.draw(targetVertices, &baseTileSet);
	
	//draw top border tiles
	for (auto i = 0; i < gridSize.x; ++i)
	{
		for (auto j = 0; j < gridSize.y; ++j)
		{
			sf::Vertex* quad = &targetVertices[(i + j * gridSize.x) * 4];
			sf::Uint8 alpha = 0;
			if (j > 0)
			{
				// define texture coordinates and alpha
				unsigned int v = mapping[i + (j - 1) * gridSize.x];		// row: type identification
				alpha = (v != mapping[i + j * gridSize.x]) ? 255 : 0;
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
	for (auto i = 0; i < gridSize.x; ++i)
	{
		for (auto j = 0; j < gridSize.y; ++j)
		{
			sf::Vertex* quad = &targetVertices[(i + j * gridSize.x) * 4];
			sf::Uint8 alpha = 0;
			if (i > 0)
			{
				// define texture coordinates and alpha
				unsigned int v = mapping[i  - 1 + j * gridSize.x];		// row: type identification
				alpha = (v != mapping[i + j * gridSize.x]) ? 255 : 0;
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
	for (auto i = 0; i < gridSize.x; ++i)
	{
		for (auto j = 0; j < gridSize.y; ++j)
		{
			// get a pointer to the current quad
			sf::Vertex* quad = &targetVertices[(i + j * gridSize.x) * 4];
			sf::Uint8 alpha = 0;
			if (i > 0 && j > 0)
			{
				// define texture coordinates and alpha
				unsigned int v = mapping[i - 1 + (j - 1) * gridSize.x];		// row: type identification
				alpha = (v != mapping[i + (j - 1) * gridSize.x] || v != mapping[i - 1 + j * gridSize.x]) ? 255 : 0;
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
