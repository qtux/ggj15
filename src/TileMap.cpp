#include "TileMap.hpp"

#include "global.hpp"

TileMap::TileMap(sf::Vector2u tileSize, sf::Vector2u gridSize, std::vector<bool> solid):
	tileSize(tileSize),
	gridSize(gridSize),
	_texture(gb::textureManager.getTexture("./img/template.png", false)),
	_solid(std::move(solid))
{
	// requires a gridSize.x * tileSize.x times gridSize.y * tileSize.y texture
	_vertices.setPrimitiveType(sf::Quads);
	_vertices.resize(gridSize.x * gridSize.y * 4);
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
			_texture,
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
			_texture,
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
	
	// swap collision data and uv coordinates TODO remove comment
	//std::swap(_solid[firstIndex], _solid[secondIndex]);
	std::swap(firstQuad[0].texCoords, secondQuad[0].texCoords);
	std::swap(firstQuad[1].texCoords, secondQuad[1].texCoords);
	std::swap(firstQuad[2].texCoords, secondQuad[2].texCoords);
	std::swap(firstQuad[3].texCoords, secondQuad[3].texCoords);
	
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
	_movingTiles.remove_if(lambda);
}

void TileMap::draw(sf::RenderTarget& target)
{
	target.draw(_vertices, &_texture);
	for (auto& ani: _movingTiles)
	{
		target.draw(*ani.sprite);
	}
}
